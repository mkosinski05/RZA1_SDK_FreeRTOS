/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined EAGLVIEW__H_
#define EAGLVIEW__H_

#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/EAGL.h>
#include "eC_Types.h"

///@cond 
//cond command exclude part of a file from processing.
/**
    This class implements the needed methods from UIView.
 */
@interface EAGLView : UIView
{
@private
    GLint m_iBackingWidth; ///< Width
    GLint m_iBackingHeight; ///< Height

    EAGLContext *m_pContext; ///< Pointer to EAGLContext of this view

    GLuint m_uiViewRenderbuffer; ///< Stores renderbuffer
    GLuint m_uiViewFramebuffer; ///< Stores framebuffer
    GLuint m_uiDepthRenderbuffer; ///< Stores depthbuffer

    eC_UInt m_uiBtnPressedStartTime; ///< Start time when touch happened
    eC_Int m_iBtnPressedMouseX; ///< X Position of the touch
    eC_Int m_iBtnPressedMouseY; ///< Y Position of the touch

    /// The supported iPhone orientations.
    enum IPhoneOrientation_t
    {
        /// Portrait mode.
        IPO_PORTRAIT,
        /// Landscape mode, corresponding to the plist property 'UIInterfaceOrientation'.
        IPO_LANDSCAPE_LEFT
    };

    eC_UInt m_uiScreenHeight; ///< The screen height
    IPhoneOrientation_t m_eOrientation; ///< Screen orientation
}

@property (nonatomic, retain) EAGLContext *m_pContext;

/**
    Creates the framebuffer.
    @return YES if sucessfully created.
 */
- (BOOL)createFramebuffer;

/**
    Destroys the framebuffer.
 */
- (void)destroyFramebuffer;

/**
    Swaps the buffers.
 */
- (void)drawView;

/**
    Returns the current view framebuffer.
    @return current view framebuffer.
 */
- (GLuint)getViewFramebuffer;

/**
    Returns the orientation.
    @return current orientation.
 */
- (IPhoneOrientation_t)getOrientation;

/**
    Sets the orientation.
    @param eOrientation the orientation to set.
 */
- (void)setOrientation:(IPhoneOrientation_t)eOrientation;

/**
    Gets and return the correct calculated touch points.
    @param[in] touches NSSet of touches as received from iOS.
    @param[in,out] iTouchPosX Calculated X position of the touch.
    @param[in,out] iTouchPosY Calculated Y position of the touch.
 */
- (void)getTouchPoints:(NSSet*) touches:(eC_Int&) iTouchPosX:(eC_Int&) iTouchPosY;

@end

#endif //EAGLVIEW__H_
///@endcond
