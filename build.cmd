cl /EHsc /nologo /W4 /WX /std:c++20 /Zi /F 8000000 /I %~dp0Includes main.cpp
:: /O2 max speed
:: /F 8000000 increase stack size to 8MB from 1MB to make recursion problems a little bit easier