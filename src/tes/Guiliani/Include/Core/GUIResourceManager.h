/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIRESOURCEMANAGER__H_
#define GUIRESOURCEMANAGER__H_

#include "eC_String.h"
#include "GUIFontResource.h"
#include "GUISoundResource.h"
#include "GUIImageResource.h"
#include "GUIGeneralResource.h"
#include "FntWrap.h"
#include "GUIBackgroundImageLoaderThread.h"

class CGUIObject;

/// Retrieve pointer to resource manager instance.
#define GETRESMANAGER        CGUIResourceManager::GetInstance()

/// Version of dialog files.
const eC_Int DIALOG_FILE_VERSION = 1;

/// Manages resource ID mappings for fonts, images, sounds, etc.

/**
@section sec_resman_overview Overview
A GUI usually consists of a variety of resources such as images, fonts and sounds which it uses to visualize itself.
An efficient management of these resources is required to optimize the memory and CPU usage of your application.
This is critical especially on embedded systems, where system resources are typically very limited.

Guiliani enables you to share resources among various objects within the GUI and allows you to tune the way in which
they are loaded / unloaded so that it best matches your target setup. Usually this will be a trade-off between
loading-times and amount of memory usage.

@section sec_resman_the_resource_manager The Resource Manager
The ResourceManager maps abstract resource identifiers (ImageResource_t, SoundResource_t, FontResource_t) to file
names. For example calling RegisterImageResource("FancyLogo.png", IMG_MY_FANCY_LOGO) will make the image in FancyLogo.png
available throughout your application under the abstract ID IMG_MY_FANCY_LOGO. These relations can be set manually or can be
read from an initfile. Objects can request and free resources by ID. The ResourceManager keeps a reference count of all
resources and causes their loading (upon first request) or unloading (if a resource is no longer in use by any object.)

Typical use-case (for images):

-# Create a new ResourceID by adding it to UserImageResource.h  (e.g. IMG_MY_FANCY_LOGO)
-# Call RegisterImageResourcesFromFile() in order to load mappings of resource IDs and resource files
   Alternatively, register the images by calling RegisterImageResource() manually.
-# Before usage, call appropriate loader function, e.g. RequestImageResource(), so that the resource data
   is loaded into RAM if needed.
-# Use the resource (e.g. Blit it using GETGFX.BlitImg() )
-# Free the resource data by calling appropriate unloader function, e.g. FreeImageResource(), so that
   resource data will be freed if no longer in use by the application.

   Example usage:
@code
#include "GUIResourceManager.h"
    // Register Image
    GETRESMANAGER.RegisterImageResource("FancyLogo.png", IMG_MY_FANCY_LOGO);
    // Request image (this will load it, if it is not already present in memory)
    GETRESMANAGER.RequestImageResource(IMG_MY_FANCY_LOGO);
    // Use the image as required
    GETGFX.BlitImg( IMG_MY_FANCY_LOGO, 0,0);
    // free image (this will unload it, if it is not requested by anyone else anymore
    GETRESMANAGER.FreeImageResource(IMG_MY_FANCY_LOGO);
@endcode

If the application decides to instantiate an image cache (see
CGUIImageCache::CreateInstance()), loading and unloading of images is managed
by the image cache instead of the resource manager. To learn more, see
CGUIImageCacheImpl.

@section sec_resman_background_image_loading Loading images in background
If you wish to optimize the flow of your application further, you may want to use the ResourceManager's AddToPrefetchQueue() method.
This tells the framework to load the given image in a low-priority background thread, so that it is already available in memory
once it gets requested for the first time. A typical use-case for this would for example be to prefetch the images of the GUI-screen
which the user is most likely to enter next.

@section sec_resman_dynamic_resources Dynamic resources
As outlined above, there is typically one pre-defined resource ID per resource.
If an application needs to load images or other resources that were unknown at
build time, there are no pre-defined IDs for these resources. To facilitate
registering and loading of such resources, the resource manager provides the
so-called <i>dynamic</i> resource management. It takes the following steps:
-# Call RegisterDynamicImage(), RegisterDynamicSound() or RegisterDynamicFont().
   These methods return dynamically assigned resource IDs.
-# Use the returned ID exactly like pre-defined IDs for requesting and releasing
   resources.
-# After the request count of the resource reaches zero, it is (by default)
   unloaded and its information is removed from the resource manager, rendering
   the resource ID invalid. It must not be used again after that.

In some cases, it might be necessary to keep dynamic resources around for
reloading even if their request count reaches zero in-between. To instruct the
resource manager to keep resource information, call one of the following methods:
KeepDynamicImages(), KeepDynamicSounds() or KeepDynamicFonts().
*/

class CGUIResourceManager
{
    friend class CGUI;
public:
    /// return pointer to resource manager instance
    static inline CGUIResourceManager& GetInstance() { return ms_kResManager;}

#ifdef GUILIANI_WRITE_GUI
    /**
        Writes all registered ImageResources of the GUI to File. A CGUIStreamWriter has to be initialized.
        @param kPath Path to the destination file.
        @throw CGUIException if destination file could not be opened.
        @see CGUIStreamWriter
        @return Always True
    */
    eC_Bool WriteImageResourcesToFile(const eC_String& kPath);

     /**
        Writes all registered FontResources of the GUI to File. A CGUIStreamWriter has to be initialized.
        @param kPath Path to the destination file.
        @throw CGUIException if destination file could not be opened.
        @see CGUIStreamWriter
        @return Always True
    */
    eC_Bool WriteFontResourcesToFile(const eC_String& kPath);

    /**
        Writes all registered SoundResources of the GUI to File. A CGUIStreamWriter has to be initialized.
        @param kPath Path to the destination file.
        @throw CGUIException if destination file could not be opened.
        @see CGUIStreamWriter
        @return Always True
    */
    eC_Bool WriteSoundResourcesToFile(const eC_String& kPath);

    /**
        Writes all registered GeneralResources of the GUI to File. A
        CGUIStreamWriter has to be initialized.

        @param rkPath Path to the destination file.
        @throw CGUIException If destination file could not be opened.
        @see CGUIStreamWriter
        @return Always True
     */
    eC_Bool WriteGeneralResourcesToFile(const eC_String& rkPath);

    /**
        Writes a GUIObject (and in case of a CompositeObject all of its children) to streaming file.
        This file can be used with the GUIFactory to create the GUI at runtime.
        @param pkStreamingFile Filename and path of destination file.
        @param pkDialog Pointer to the dialog which should be written
    */
    void WriteDialogToFile(const eC_String& pkStreamingFile, CGUIObject* pkDialog);

    /**
        Writes a GUIObject (and in case of a CompositeObject all of its children) to streaming file.
        This file can be used with the GUIFactory to create the GUI at runtime.
        @param pkFile eC_File* of destination file.
        @param pkDialog Pointer to the dialog which should be written
    */
    void WriteDialogToFile(eC_File *pkFile, CGUIObject* pkDialog);
#endif

#ifdef GUILIANI_STREAM_GUI
    /**
        Reads ImageResources and their paths from file and registers them. A CGUIStreamReader has to be initialized.
        @param kPath Path to the source file.
        @see CGUIStreamReader
        @return Always True
    */
    eC_Bool RegisterImageResourcesFromFile(const eC_String& kPath);
    /**
        Reads FontResources and their attributes from file and registers the fonts.
        GUIStreamReader has to be initialized.
        @param kPath Path to the source file.
        @throw CGUIException if source file could not be opened, or errors occurred while reading the file.
        @see CGUIStreamReader
        @return Always True
    */
    eC_Bool RegisterFontResourcesFromFile(const eC_String& kPath);
    /**
        Reads SoundResources and their attributes from file and registers the sounds.
        GUIStreamReader has to be initialized.
        @param kPath Path to the source file.
        @throw CGUIException if source file could not be opened, or errors occurred while reading the file.
        @see CGUIStreamReader
        @return Always True
    */
    eC_Bool RegisterSoundResourcesFromFile(const eC_String& kPath);

    /**
        Reads GeneralResources and their attributes from file and registers the
        resources. GUIStreamReader has to be initialized.

        @param rkPath Path to the source file.
        @throw CGUIException If source file could not be opened, or errors
        occurred while reading the file.
        @see CGUIStreamReader
        @return Always True
     */
    eC_Bool RegisterGeneralResourcesFromFile(const eC_String& rkPath);

#endif

    /**
        This method generates a dynamic image identifier for the given image and automatically registers it.
        @param kPath to the image file.
        @param bPermanent If the flag is true the ResourceManager will not unload the resource
                          when the request-count is decremented to 0 (false by default).
        @return ImageID under which this image will now be available.
    */
    ImageResource_t RegisterDynamicImage(const eC_String& kPath, const eC_Bool bPermanent=false);

    /**
        This method generates a dynamic sound identifier for the given sound and automatically registers it.
        @param kPath to the sound file.
        @param bPermanent If the flag is true the ResourceManager will not unload the resource
                          when the request count is decremented to 0 (false by default).
        @return SoundID under which this sound will now be available.
    */
    SoundResource_t RegisterDynamicSound(const eC_String& kPath, const eC_Bool bPermanent=false);

    /**
        This method generates a dynamic font identifier for the given font and automatically registers it.
        @param kPath to the font file.
        @param uiFontSize Size of the font
        @param eFontStyle as defined in CFntWrap::GUIFont_t
        @param bPermanent If the flag is true the ResourceManager will not unload the resource. (true by default).
        @return FontID under which this font will now be available.
    */
    FontResource_t RegisterDynamicFont( const eC_String& kPath, eC_UInt uiFontSize,
                                        const CFntWrap::GUIFont_t &eFontStyle, eC_Bool bPermanent=true);

    /**
       This method generates a dynamic general resource identifier for the given
       resource file and automatically registers it.

       @param rkPath To the general resource file.
       @param bPermanent If the flag is true the ResourceManager will not unload
        the resource. (False is default).
       @return ResourceID under which this general resource will now be available.
     */
    GeneralResource_t RegisterDynamicGeneralResource(const eC_String& rkPath,
        eC_Bool bPermanent = false);

    /** Enables or disables keeping of dynamic image information.
        By default, a dynamic image is removed once the request count for the
        image reaches zero. By enabling <i>keeping</i>, the image path is kept
        even if the request count reaches zero. At this point, the image is
        unloaded if it is not permanent, but keeping the path makes it possible
        to reload it by requesting the image again using the same dynamically
        assigned ID. The downside of this behaviour is that (very small)
        portions of memory stay occupied for the image path and other image
        information.
        @param bKeep True for enabling keeping, false for disabling it. When
               disabling, each dynamic image path whose request count is zero
               is removed.
      */
    void KeepDynamicImages(eC_Bool bKeep);

    /** Enables or disables keeping of dynamic sound information.
        By default, a dynamic sound is removed once the request count for the
        sound reaches zero. By enabling <i>keeping</i>, the sound path is kept
        even if the request count reaches zero. At this point, the sound is
        unloaded if it is not permanent, but keeping the path makes it possible
        to reload it by requesting the sound again using the same dynamically
        assigned ID. The downside of this behaviour is that (very small)
        portions of memory stay occupied for the sound path and other sound
        information.
        @param bKeep True for enabling keeping, false for disabling it. When
               disabling, each dynamic sound path whose request count is zero
               is removed.
      */
    void KeepDynamicSounds(eC_Bool bKeep);

    /** Enables or disables keeping of dynamic font information.
        By default, a dynamic font is removed once the request count for the
        font reaches zero. By enabling <i>keeping</i>, the font path is kept
        even if the request count reaches zero. At this point, the font is
        unloaded if it is not permanent, but keeping the path makes it possible
        to reload it by requesting the font again using the same dynamically
        assigned ID. The downside of this behaviour is that (very small)
        portions of memory stay occupied for the font path and other font
        information.
        @param bKeep True for enabling keeping, false for disabling it. When
               disabling, each dynamic font path whose request count is zero
               is removed.
      */
    void KeepDynamicFonts(eC_Bool bKeep);

    /**
       Enables or disables keeping of dynamic abstract resources. It depends
       on the implementation of the resource file interpreter how to cache
       the resource then, just the flag of keeping of dynamic resources is
       provided by ResourceManager.

       @param bKeep True for forcing the given interpreter to keep the
        resource data, False otherwise.
     */
    void KeepDynamicGeneralResources(eC_Bool bKeep);

    /**
        Enable and disable preloading of permanent data.
        If enabled, permanent data is loaded on registration.
        Attention! If you enable preloading after registration of permanent data,
        this data will not be loaded if it is requested. Set this flag before any
        registration to avoid this!
        @param bPreLoadPermanentData True if permanent
    */
    inline void EnablePreloading(const eC_Bool bPreLoadPermanentData){ m_bPreLoadPermanentData = bPreLoadPermanentData; }

    /**
        Use this method to add an image resource (which you expect to be required soon) to the prefetch queue.
        The image will be loaded in a low-priority background thread, so that the loading does not interrupt the GUI execution
        but the image will be already available once it gets requested.
        Note that prefetching does not automatically request an image. This means that its reference count remains zero and
        it may be unloaded any time, until you explicitly request it.
        @param eID ID of the Image which will be loaded in background.
    */
    void AddToPrefetchQueue( const ImageResource_t eID);

    /**
        Use this method to remove an image resource from the prefetch queue.
        If the image ID is still in the prefetch queue it will be simply removed.
        If it has been already loaded and is not used so far,
        the image will be marked as released and unloaded during the next call of PrefetchNow()
        @param eID ID of the Image which will be loaded in background.
        @return True if the image resource has been removed from the queue, False if it is not in the list and could not been unloaded
    */
    eC_Bool RemoveFromPrefetchQueue( const ImageResource_t eID);

    /** Sets the time in milliseconds that has to pass between prefetching one
        image and the next in the queue. By default this is set to 50 ms.
        @param uiDelay Delay between image prefetch calls in milliseconds.
      */
    void SetPrefetchDelay(eC_UInt uiDelay);

    /**
        Use this method to request an image resource.
        If a resource is requested for the first time and an image was registered before, the ResourceManager
        will call the GfxWrapper to load the resource.
        If the GFXWrapper could not load the resouce, it will use GUILOG to issue a warning.
        @param eImageID ID of the Requested Image.
    */
    void RequestImageResource(const ImageResource_t eImageID);

    /**
        Use this method to request a font resource.
        If a resource is requested for the first time and a font was registered before, the ResourceManager
        will call the GfxWrapper to load the resource.
        If the GFXWrapper could not load the resouce, it will use GUILOG to issue a warning.
        @param eFontID ID of the requested font.
    */
    void RequestFontResource(const FontResource_t eFontID);

    /**
        Use this method to request a sound resource.
        If a resource is requested for the first time and a sound was registered before, the ResourceManager
        will call the SoundWrapper the resource.
        If the SoundWrapper could not load the resouce, it will use GUILOG to issue a warning.
        @param eSoundID ID of the requested sound.
    */
    void RequestSoundResource(const SoundResource_t eSoundID);

    /**
       Use this method to request a general resource.

       If a resource is requested for the first time and a resource was
       registered before, the ResourceManager will call the given interpreter
       to handle the resource.

       If the interpreter could not load the resource, it will use GUILOG to
       issue a warning.

       @param eResourceID ID of the requested general resource.
     */
    void RequestGeneralResource(const GeneralResource_t eResourceID);

    /**
        Use this method to free an image resource.
        If the requestcounter of a resource is 0 and the resource is not declared as permanent,
        the ResourceManager will call the GFXWrapper to unload the resource.
        If the resource was dynamic, it's ID will be freed to make it usable again.
        @param eImageID ID of the image which is no longer in use.
   */
    void FreeImageResource(const ImageResource_t eImageID);

    /**
        Use this method to free a font resource.
        If the requestcounter of a resource is 0 and the resource is not declared as permanent,
        the ResourceManager will call the GFXWrapper to unload the Resource.
        If the resource was dynamic, it's ID will be freed to make it usable again.
        @param eFontID ID of the font which is no longer in use.
   */
    void FreeFontResource(const FontResource_t eFontID);

    /**
        Use this method to free an sound resource.
        If the requestcounter of a resource is 0 and the resource is not declared as permanent,
        the resourcemanager will call the SoundWrapper to unload the resource.
        If the resource was dynamic, it's ID will be freed to make it usable again.
        @param eSoundID id of the sound which is no longer in use.
    */
    void FreeSoundResource (const SoundResource_t eSoundID);

    /**
       Use this method to free a general resource.

       If the referencecounter of a resource is 0 and the resource is not
       declared as permanent, the ResourceManager will call the interpreters
       unload method.

       If the resource was dynamic, it's ID will be freed to make it usable
       again.

       @param eResourceID ID of the general resource is no longer in use.
     */
    void FreeGeneralResource(const GeneralResource_t eResourceID);

    /** Use this method to register an image resource ID to a path.
        @param kPath to the image file.
        @param eImageID ID which should be mapped to the image file.
        @param bPermanent If the flag is true the ResourceManager will not unload the resource
        if the request-count is decremented to 0 (false by default).
    */
    void RegisterImageResource(const eC_String& kPath, const ImageResource_t eImageID, const eC_Bool bPermanent=false);

    /** Retrieves the font size of font.
        @param eFontID id of the font.
        @return Font size in ppi or 0 if the font ID is invalid.
    */
    eC_UInt GetFontSize(const FontResource_t &eFontID) const;

    /** Retrieves the path of the registered font.
        @param eFontID id of the font.
        @return The font path or an empty string if no font with that ID was registered or the font ID is invalid.
    */
    eC_String GetFontPath(const FontResource_t &eFontID) const;

    /** Retrieves the style of the registered font.
        @param eFontID id of the font.
        @return the font style, CFntWrap::FNT_NORMAL if no font for this id is registered or the font ID is invalid.
    */
    CFntWrap::GUIFont_t GetFontStyle(const FontResource_t &eFontID) const;

    /** Set the style for the font.
        @param eFontID ID of the font.
        @param eStyle The font style.
    */
    void SetFontStyle(const FontResource_t &eFontID, const CFntWrap::GUIFont_t& eStyle);

    /** Use this method to register a font resource ID to a path.
        @param kPath to the font file.
        @param eFontID ID which should be mapped to the font.
        @param uiFontSize Size of the font
        @param eFontStyle as defined in CFntWrap::GUIFont_t
        @param bPermanent If the flag is true the ResourceManager will not unload the resource. (true by default).
    */
    void RegisterFontResource(const eC_String& kPath, const FontResource_t eFontID, const eC_UInt uiFontSize,
                              const CFntWrap::GUIFont_t &eFontStyle, const eC_Bool bPermanent=true);

    /** Use this method to register a sound resource ID to a path.
        @param pcPath path to the soundFile.
        @param eSoundID id which should be mapped to the soundfile.
        @param bPermanent if the flag is true the resourcemanager will not unload the resource.
    */
    void RegisterSoundResource(const eC_String& pcPath, const SoundResource_t eSoundID, const eC_Bool bPermanent = false);

    /** Use this method to register a general resource ID to a path.
        @param rkPath Path to the general resource file.
        @param eResourceID ID which should be mapped to the resource file.
        @param bPermanent If the flag is true, the ResourceManager will not
        unload the resource.
     */
    void RegisterGeneralResource(const eC_String& rkPath,
        const GeneralResource_t eResourceID,
        eC_Bool bPermanent = false);

    /** Get the path of an image id.
        @param eImageID ID of the image.
        @return Path of the image file that the id is registered to.
    */
    eC_String GetImagePath (const ImageResource_t& eImageID) const;

    /** Get the request count of an image id.
        @param eImageID ID of the image.
        @return Request count of the image or -1 if the image ID is invalid.
    */
    eC_Int GetImageRequestCount(const ImageResource_t& eImageID) const;

    /** Get the request count of a font id.
        @param eFontID ID of the font.
        @return Request count of the font or -1 if the font ID is invalid.
    */
    eC_Int GetFontRequestCount(const FontResource_t& eFontID) const;

    /** Get the request count of a sound id.
        @param eSoundID ID of the sound.
        @return Request count of the sound or -1 if the sound ID is invalid.
    */
    eC_Int GetSoundRequestCount(const SoundResource_t& eSoundID) const;

    /** Get the request count of a general resource id.
        @param eGeneralResourceID ID of the general resource.
        @return Request count of the general resource or -1 if the resource ID is invalid.
    */
    eC_Int GetGeneralResourceRequestCount(const GeneralResource_t& eGeneralResourceID) const;

    /** Get the path of a sound resource.
        @param eSoundID ID of the sound.
        @return Path of the sound file that the ID is registered to.
    */
    eC_String GetSoundPath(const SoundResource_t& eSoundID) const;

    /** Get the path of a general resource.
        @param eResourceID ID of the general resource.
        @return Path of the general resource file that the ID is registered to.
     */
    eC_String GetGeneralResourcePath(const GeneralResource_t& eResourceID) const;

    /** Checks whether an ID was created for a dynamically registered image.
        @param eImageID The image ID to be checked.
        @return True if eImageID is a dynamic ID, otherwise False.
    */
    eC_Bool IsDynamicImage(ImageResource_t eImageID);

    /** Checks whether an ID was created for a dynamically registered sound.
        @param eSoundID The sound ID to be checked.
        @return True if eSoundID is a dynamic ID, otherwise False.
    */
    eC_Bool IsDynamicSound(SoundResource_t eSoundID);

    /** Checks whether an ID was created for a dynamically registered font.
        @param eFontID The font ID to be checked.
        @return True if eFontID is a dynamic ID, otherwise False.
    */
    eC_Bool IsDynamicFont(FontResource_t eFontID);

    /** Checks whether an ID was created for a dynamically registered general resource.
        @param eResourceID The general resource ID to be checked.
        @return True if eResourceID is a dynamic ID, otherwise False.
     */
    eC_Bool IsDynamicGeneralResource(GeneralResource_t eResourceID);

    /** Checks whether the permanent flag for an image is set.
        @param eImageID The image to be checked.
        @return True if the image is permanent, otherwise False.
      */
    eC_Bool IsImagePermanent(const ImageResource_t& eImageID) const;

    /** Checks whether the permanent flag for a font is set.
        @param eFontID The font to be checked.
        @return True if the font is permanent, otherwise False.
      */
    eC_Bool IsFontPermanent(const FontResource_t& eFontID) const;

    /** Checks whether the permanent flag for a sound is set.
        @param eSoundID The sound to be checked.
        @return True if the sound is permanent, otherwise False.
      */
    eC_Bool IsSoundPermanent(const SoundResource_t& eSoundID) const;

    /** Checks whether the permanent flag for a general resource is set.
        @param eResourceID The general resource to be checked.
        @return True if the general resource is permanent, otherwise False.
     */
    eC_Bool IsGeneralResourcePermanent(const GeneralResource_t& eResourceID) const;

    /** Get the number of images contained in the prefetch queue of the background image loader.
        @return The remaining number of images in the prefetch queue.
    */
    eC_UInt GetNofImagesInPrefetchList() const;

    /// Only available in debug builds: Prints the count of reloads for each image
    void PrintReloadCounts();

    /// Only available in debug builds: Prints the count of currently requested images
    void PrintRequestCounts();

#ifndef eC_TARGET_ENV_POCKET
private:
#else
public: // workaround for the broken eVC4 compiler: it doesn't find a private destructor
#endif
    /** Destructor */
    ~CGUIResourceManager();

private:
    /**
        Use this method to unload an image.
        All checks concerning permanent flags, image cache, dynamic images, etc. are handled.
        This method does NOT do any check wether the image id is in the array bounds or not. This has to be done by the callee!
        @param eImageID ID of the image which should be unloaded.
    */
    void UnloadImage(ImageResource_t eImageID);

    /** Should only be called by the destructor. As a workaround for memory leak
        detection ~CGUI() may call it too, but it is forbidden to be called by
        other code.
    */
    void DeInit();

    /// Should only be called by the constructor
    void Init();

    /** Standard-constructor */
    CGUIResourceManager();

    /** Copy-constructor.
    *   Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
    */
    CGUIResourceManager(const CGUIResourceManager& kSource);

    /** Operator= method. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
        @return This object.
    */
    CGUIResourceManager& operator=(const CGUIResourceManager& kSource);

    /// static ResourceManager instance
    static CGUIResourceManager ms_kResManager;

    // Structures to hold Resource-information.
    // Yes, this is currently identical for some Resource-types, but still kept separate for future extensions.
    struct ImageEntry_t
    {
        /// The name/path of the image.
        eC_String*  pkPath;
        /// Indicates how many times the image is requested. A request count of zero will allow the image to be unloaded.
        eC_Int      iRequestCount;
        /// indicates whether the font is permanently registered. This prevents the image from being unloaded when its request count reaches zero.
        eC_Bool     bPermanent;
    };

    struct SoundEntry_t
    {
        /// The name/path of the sound
        eC_String*  pkPath;
        /// Indicates how many times the sound is requested. A request count of zero will allow the sound to be unloaded.
        eC_Int      iRequestCount;
        /// indicates whether the font is permanently registered. This prevents the sound from being unloaded when its request count reaches zero.
        eC_Bool     bPermanent;
    };

    struct FontEntry_t
    {
        /// The name/path of the font.
        eC_String*          pkFontName;
        /// The font size.
        eC_UInt             uiFontSize;
        /// The font style.
        CFntWrap::GUIFont_t eFontStyle;
        /// indicates how many times the font is requested. A request count of zero will allow the font to be unloaded.
        eC_Int              iRequestCount;
        /// indicates whether the font is permanently registered. This prevents the font from being unloaded when its request count reaches zero.
        eC_Bool             bPermanent;
    };

    struct GeneralResourceEntry_t
    {
        /// The path of the resource file.
        eC_String* pkResourceFilePath;
        /// Indicates how many times the resource file is requested.
        eC_Int     iRequestCount;
        /// Indicates whether the resource file is permanently registered.
        eC_Bool    bPermanent;
    };

    eC_TArray<ImageEntry_t> m_kaImageTable;
    eC_TArray<FontEntry_t>  m_kaFontTable;
    eC_TArray<SoundEntry_t> m_kaSoundTable;
    eC_TArray<GeneralResourceEntry_t> m_kaGeneralResTable;

    eC_TListDoubleLinked<ImageResource_t> m_aeReleasedPreloadedImages;

    eC_Bool m_bPreLoadPermanentData;

    /// Whether dynamic images are removed when request count reaches 0.
    eC_Bool m_bKeepDynamicImages;

    /// Whether dynamic sounds are removed when request count reaches 0.
    eC_Bool m_bKeepDynamicSounds;

    /// Whether dynamic fonts are removed when request count reaches 0.
    eC_Bool m_bKeepDynamicFonts;

    /**
       Whether dynamic general resources are removed when request count
       reaches 0.
     */
    eC_Bool m_bKeepDynamicGeneralResources;

    /// This will be called internally by the GUI to unblock the background image-loading thread, if this feature is activated.
    void PrefetchNow();
};



/** The BackgroundImageLoader class manages the loading of images in a background thread.
    The framework manages this class internally so that it is never accessed directly by user-code.
    The BackgroundImageLoader is responsible for handling the list of images which are to be prefetched and
    for creating / destroying the BackgroundImageLoaderThread and to unblock / block the thread once new
    images are added to the prefetch-list.
    @brief Manages the loading of images in a background thread
*/
class CGUIBackgroundImageLoader
{
    // Grants friend access to other classes involved in background-image-loading
    friend class CGUIResourceManager;
    friend class CGUIBackgroundImageLoaderThread;

    /** This returns an instance of CGUIBackgroundImageLoader. When called for the first time, it
        automatically creates an instance of the class. */
    static CGUIBackgroundImageLoader* GetInstance();

    // Checks whether an instance of CGUIBackgroundImageLoader already exists.
    static inline eC_Bool IsAvailable() { return NULL != ms_pkBGImageLoaderInstance; }

    CGUIBackgroundImageLoader();

    // private copy constructor
    CGUIBackgroundImageLoader(const CGUIBackgroundImageLoader& ref) {}

    ~CGUIBackgroundImageLoader();

    // Adds an image to the internal list of images which are to be loaded in the background thread
    eC_Bool AddImageToPrefetchList( const ImageResource_t eID, const eC_String& kPath);

    // Removes an image from the internal list of images which are to be loaded in the background thread
    eC_Bool RemoveImageFromPrefetchList( const ImageResource_t eID, const eC_String& kPath);

    /** Used this method to get the image ID the BackgroundImageLoader is currently loading.
        @param ID of the currently loaded image ID, if no image is loaded at the moment DUMMY_IMAGE is returned
    */
    ImageResource_t GetUsedImagedID() const {return m_eUsedImage;}

    inline eC_UInt GetNOFImagesInPrefetchList() const { return m_kaPrefetchImageList.GetQuantity(); }

    // Internal storage structure for image-related information which is required for background loading
    struct PrefetchImage_t
    {
        PrefetchImage_t(const ImageResource_t eID = DUMMY_IMAGE, const eC_String& rkPath = "") :
                                    m_eID(eID), m_kPath(rkPath){}

        ImageResource_t     m_eID;
        eC_String           m_kPath;

        bool operator== (const PrefetchImage_t& rkCompare) const
        {
            if ((m_eID == rkCompare.m_eID) &&
               (m_kPath == rkCompare.m_kPath))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    };

    // List of images which are to be prefetched
    eC_TListDoubleLinked< PrefetchImage_t>  m_kaPrefetchImageList;

    // Semaphore for locking access to the internal list of images
    eC_Semaphore                     m_kPrefetchImageListSema;
    // Semaphore which is used for blocking / unlocking the thread if new images are added to the list.
    eC_Semaphore                     m_kImageLoaderThreadSema;

    // Pointer to the background thread instance.
    CGUIBackgroundImageLoaderThread*        m_pBackgroundImageLoaderThread;
    // Pointer to instance of this class.
    static CGUIBackgroundImageLoader*       ms_pkBGImageLoaderInstance;

    // ID of the currently loading Image, if nothing is loaded DUMMY_IMAGE
    ImageResource_t m_eUsedImage;
};


#ifndef GUILIANI_GUILIANILIB
#include "GUIPropertyResource.h"

/** Helper function which registers all static resources within the CGUIResourceManager
    Call this from your application's initialization code. */
inline void RegisterStaticResources()
{
// Register properties.
#define ENTRY(prop_name, prop_value, prop_type) GETPROPHDL.SetGlobalProperty(prop_name, prop_value, prop_type); 
    USER_PROPERTY_TABLE
#undef ENTRY

// Register images.
#define ENTRY(img, img_name, img_permanent)  GETRESMANAGER.RegisterImageResource(img_name, img, img_permanent);
    USER_IMAGE_TABLE
#undef ENTRY
        
// Register fonts.
#define ENTRY(fnt, fnt_name, fnt_size, fnt_style, fnt_permanent)  GETRESMANAGER.RegisterFontResource(fnt_name, fnt, fnt_size, static_cast<CFntWrap::GUIFont_t>(fnt_style), fnt_permanent);
    USER_FONT_TABLE
#undef ENTRY

// Register general resources.
#define ENTRY(res, res_name, res_permanent)  GETRESMANAGER.RegisterGeneralResource(res_name, res, res_permanent);
    USER_GENERAL_RESOURCES_TABLE
#undef ENTRY

// Register sounds.
#define ENTRY(snd, snd_name, snd_permanent)  GETRESMANAGER.RegisterSoundResource(snd_name, snd, snd_permanent);
    USER_SOUND_TABLE
#undef ENTRY
}

/** Register all standard Guiliani images so that they may be requested by controls.*/
inline void RegisterGuilianiStandardImages()
{
#define ENTRY(img, img_name, img_permanent)  GETRESMANAGER.RegisterImageResource("GuilianiResources/"  img_name, img, img_permanent);
    IMAGE_TABLE
#undef ENTRY
}
/// For downward compatibility reasons. You can either call RegisterGuilianiStandardImages() above or use the macro REGISTER_GUI_STANDARDCONTROL_IMAGES.
#define REGISTER_GUI_STANDARDCONTROL_IMAGES RegisterGuilianiStandardImages();
#endif

#endif

