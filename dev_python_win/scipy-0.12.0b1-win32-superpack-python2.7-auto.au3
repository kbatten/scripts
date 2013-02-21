ConsoleWrite("installing scipy-0.12.0b1-win32-superpack-python2.7.exe" & @CRLF)
if ($CmdLine[0] == 1) Then
   ConsoleWrite("running: " & $CmdLine[1] & "\scipy-0.12.0b1-win32-superpack-python2.7.exe /S" & @CRLF)
   Run($CmdLine[1] & "\scipy-0.12.0b1-win32-superpack-python2.7.exe /S")
Else
   ConsoleWrite("running: scipy-0.12.0b1-win32-superpack-python2.7.exe /S" & @CRLF)
   Run("scipy-0.12.0b1-win32-superpack-python2.7.exe /S")
EndIf

ConsoleWrite("WinWaitActive(""Setup"")" & @CRLF)
WinWaitActive("Setup")
ConsoleWrite("!n" & @CRLF)
Send("!n")
Sleep(250)
ConsoleWrite("!n" & @CRLF)
Send("!n")
Sleep(250)
ConsoleWrite("!n" & @CRLF)
Send("!n")
Sleep(250)
ConsoleWrite("WinWaitActive(""Setup"", ""Finish"")" & @CRLF)
WinWaitActive("Setup", "Finish")
ConsoleWrite("Send(""{ENTER}"")" & @CRLF)
Send("{ENTER}")
