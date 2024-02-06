
def loadModule():
    if any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAM9X7", "SAMA7"]):
        print("libcamera module loaded to support " + str(Variables.get("__PROCESSOR")))
        component = Module.CreateComponent("vision_camera", "libCamera", "/Vision/Middleware/", "libcamera.py")
        component.setDisplayType("Camera Library")
        component.addDependency("vision_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        component.addDependency("vision_driver_isc", "DRV_ISC", False, True)                
        component.addDependency("vision_image_sensor_driver", "DRV_IMAGE_SENSOR", False, True)
    else:
        print("libcamera module not loaded.  No support for " + str(Variables.get("__PROCESSOR")))
