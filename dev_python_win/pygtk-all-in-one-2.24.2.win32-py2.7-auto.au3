ConsoleWrite("installing pygtk-all-in-one-2.24.2.win32-py2.7.msi" & @CRLF)
if ($CmdLine[0] == 1) Then
   ConsoleWrite("running: " & $CmdLine[1] & "\msiexec.exe /i pygtk-all-in-one-2.24.2.win32-py2.7.msi" & @CRLF)
   Run($CmdLine[1] & "\msiexec.exe /i pygtk-all-in-one-2.24.2.win32-py2.7.msi")
Else
   ConsoleWrite("running: msiexec.exe /i pygtk-all-in-one-2.24.2.win32-py2.7.msi" & @CRLF)
   Run("msiexec.exe /i pygtk-all-in-one-2.24.2.win32-py2.7.msi")
EndIf

ConsoleWrite("WinWaitActive(""Python 2.7 PyGTK 2.24.2 Setup"")" & @CRLF)
WinWaitActive("Python 2.7 PyGTK 2.24.2 Setup")
sleep(30000)
ConsoleWrite("!n" & @CRLF)
Send("!n")
Sleep(250)
ConsoleWrite("!n" & @CRLF)
Send("!n")
Sleep(250)

ConsoleWrite("WinWaitActive(""Python 2.7 PyGTK 2.24.2 Setup"", ""Install"")" & @CRLF)
WinWaitActive("Python 2.7 PyGTK 2.24.2 Setup", "Install")
ConsoleWrite("!i" & @CRLF)
Send("!i")
Sleep(250)

ConsoleWrite("WinWaitActive(""Python 2.7 PyGTK 2.24.2 Setup"", ""Finish"")" & @CRLF)
WinWaitActive("Python 2.7 PyGTK 2.24.2 Setup", "Finish")
ConsoleWrite("Send(""!f"")" & @CRLF)
Send("!f")
