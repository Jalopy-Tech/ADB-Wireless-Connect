function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install README.txt!
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut",
			"@TargetDir@/adb-wireless-connect/adb-wireless-connect.exe",
			"@StartMenuDir@/ADB Wireless Connect.lnk",
           		"workingDirectory=@TargetDir@/adb-wireless-connect");
			
	component.addOperation("CreateShortcut", 
			"@TargetDir@/adb-wireless-connect/adb-wireless-connect.exe",
			"@DesktopDir@/ADB Wireless Connect.lnk",
			"workingDirectory=@TargetDir@/adb-wireless-connect");
    }
	
	
}


