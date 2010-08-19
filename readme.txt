Pork to Sausage is a plugin for Notepad++.

I got a lot of feature requests to transform the selected text from A to B, such from a clear text to a zip text encoded in base64, or from an url encoded text to a clear (decoded) url. Some of them are easy to be implemented in C or C++ and some are not. On the other hand, those one which are complex to be implemented in C or C++ can be done in Java or .NET in very few lines thanks to their rich libraries. The idea of Pork to Sausage plugin comes from here: a plugin which is capable to pass any selected text to any command line program as input and to takes the output (the result of program) to replace the selected text. All you need is write a transformer in your favourate language. Your transformer (a small command line program) should take input from a file or stdin and generate output to a file or stdout, and that's it.

Of course you have to configure your transformer in pork2sausage.ini in order to turn your pork into the sausage(s), or vice versa.
Upto 20 commands can be customized.

Enjoy your sausage ;)

Don Ho
don.h@free.fr