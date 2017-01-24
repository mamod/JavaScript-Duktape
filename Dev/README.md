# For Development Only

This is a development helper files, it parses [duktape api](http://duktape.org/api.htm) and generates a file with all duktape api using jQuery.pl then create both  ``FunctionsMap.pl`` and ``duktape_wrapper.c`` using parse.pl which will be used in ``JavaScript::Duktape`` release


# HOW

- extract api.html file from duktape.org API
- run ``perl extract.pl``
- run ``perl parse.pl``
- copy new generated file ``FunctionsMap.pl`` to lib/JavaScript/Duktape/C
- copy content from ``duktape_wrapper.c`` to lib/JavaScript/Duktape/C/duk_perl.c
