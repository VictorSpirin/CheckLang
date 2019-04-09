CheckLang plugin for NSIS
CheckLang function checks that the language is present in Windows
For use copy CheckLang.dll to Plugins directory

Usage example in NSIS:
  CheckLang::CheckLang "0419"
  pop $R0
  ${if} $R0 == "1"
        !insertmacro MUI_LANGDLL_DISPLAY ;select language
  ${endif}

The language string must be passed as a hexadecimal language identifier that does not include the leading 0x,
and has 4 characters in length. 
For example, en-US will be passed as "0409" and en as "0009".
