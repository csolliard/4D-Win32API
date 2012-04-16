## New in Version 4.0: ##

**sys_GetTimeZoneList**

Retrieves a list of time zones defined in Windows.

TWAIN_GetSources, TWAIN_SetSource, and TWAIN_AcquireImage

Adds basic support for TWAIN imaging devices.

**sys_IsAppFrontmost**

Determines if 4D is the frontmost application.

**gui_MessageBox**

Allows access to the default Windows message boxes.

**gui_HideTaskBar** and **gui_ShowTaskBar**

Hides or shows the Windows task bar.

**gui_SetMIDOpaque** and **gui_SetMDITransparent**

Makes the 4D MDI window background opaque or transparent.

**gui_HideTitleBar** and **gui_ShowTitleBar**

Hides and shows the 4D window title bar.

**gui_MaximizeMDI**, **gui_MinimizeMDI** and **gui_RestoreMDI**

Programmatically maximizes, minimizes, and restores the 4D MDI window.

**sys_DisableTaskManager** and **sys_EnableTaskManager**

Enables or disables the ability of the user to open the Windows task manager.

## Modified in Version 4.0: ##

**gui_RespectToolbar**

Fixed several bugs when using **gui_RespectToolbar** and maximizing or minimizing the window.

**gui_SetTrayIcon**

Fixed a bug in **gui_SetTrayIcon** that would prevent the icon from being removed under Windows 7.

**sys_GetOSVersion**

The command **sys_GetOSVersion** will now recognize Windows 7, Windows Server 2008, and Windows Server 2008 Release 2.

**sys_DirectoryExists**

Updated **sys_DirectoryExists** to correctly return false if the specified drive does not exists.

**gui_RespectToolbar**

When using **gui_RespectToolbar**, the array <>TB_NOTIFICATION can no longer be used with 4Dv11.  Instead a generic call to each process that has an active toolbar will be made.

**Windows Compatibility**

Updated multiple commands to work correctly under Windows 7.

**4D v11 Compatibility**

Updated multiple commands to work correctly using 4D v11.