## New in Version 3.6.4: ##

**gui_GetWindRect**

When running with dual monitors, the command gui_GetWindRect will now optionally return coordinates relative to the actual monitor that the 4D window is displayed on, instead of the coordinates of the virtual desktop. Choosing between the existing functionality and this new functionality will be controlled by a new parameter.

**sys_GetOSVersion**

The command sys_GetOSVersion will now recognize Windows Vista and Windows Longhorn OSs.

**gui_SetWindowStyle**

Enables and disables toolbox icons.

**sys_FileExists**

Tests for the existence of a file.

**sys_DirectoryExists**

Tests for the existence of a directory.

**sys_SetPluginLanguage**

Changes language of the returned error text for sys_ShellExecute.

**sys_PrintDirect2Driver**

Sends raw data to a printer driver by name.

**sys_KillProcessByName**

Terminates processes by name

**sys_KillProcessByID**

Terminates processes by process ID.

**sys_EnumProcesses**

Enumerates processes and supplies names and IDs.

**sys_LogonUser**

Validates Windows logon credentials.