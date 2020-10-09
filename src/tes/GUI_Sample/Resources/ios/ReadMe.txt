Steps to create a valid XCode project:

1. Run Cmake to create initial XCode project and open it
2. Add new Build Phase "Copy bundle resources" for the StreamRuntime target.
3. Copy "Images.xcassets" from "resources" and replace existing Info.plist file in the project with the one from "resources" and add "Images.xcassets" to the project. Tip: right click to Info.plist and "Show in finder" will show the location. 

4. Under StreamRuntime target, General tab, select deployment target "7.0", and select apropriate icon and launch images (from added asset)

*** IF the general tab is not visible - then please check under Project, Build Settings, Base SDK - is it properly set(latest SDK). *** TODO: cmake bug - check SDKROOT
