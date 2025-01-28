# MUIExample
This project is an example on how to make MUI applications that naturally load
resources from the MUI file without any additional code. The main areas to look at
for incorporating this into your own project are `mainexample.rc`, the arguments
passed to the resource compiler, and the post-build events.

## How it works

Firstly, in `muiexample.rc` the following line is at the end of the file: 
```cpp
#include "muiexample.en-US.rc"
```

This includes the en-US resources in the main RC file, which is important for the below arguments added to the resource compiler. However, it must be stated that only one language, ideally your ultimate fallback language, is added here. For some strange reason, the resource compiler refuses to output MUI files when an RC file contains resources for multiple languages, and instead outputs one language-neutral file.
```
/g 0x0409 /q "muiconfig.xml" /fm "$(IntermediateOutputPath)%(Filename).en-US.res"
```

A rundown of the arguments:
- `/g` tells the resource compiler the ultimate fallback language to use for when the user's language doesn't have a provided MUI file. In this case, 0x0409 = 1033 = English (United States).
- `/q` gives the resource compiler a path to the XML configuration file used to determine which resource types should go in the language-neutral file or the MUI file. In this case, strings go in the MUI, and everything else goes in the language neutral file. You can find documentation on the format of the XML configuration file [here](https://learn.microsoft.com/en-us/windows/win32/intl/preparing-a-resource-configuration-file).
- `/fm` tells the resource compiler where the MUI res file should be output.

We still need to compile the other languages and link them all into MUI files, so let's take a look at the post-build events:

```cmd
if not exist "$(OutDir)en-US" md "$(OutDir)en-US"
link /dll /noentry /out:"$(OutDir)en-US\$(TargetFileName).mui" $(IntermediateOutputPath)muiexample.en-US.res
```

This is pretty simple. It creates the `en-US` folder in the output directory, and then links the en-US RES file into a resource-only DLL (`/dll /noentry`).

```cmd
rc /D "_UNICODE" /D "UNICODE" /l 0x0411 /q "muiconfig.xml" /g 0x0409 /nologo /fo"$(IntermediateOutputPath)temp.res" /fm"$(IntermediateOutputPath)muiexample.ja-JP.res" muiexample.ja-JP.rc
if not exist "$(OutDir)ja-JP" md "$(OutDir)ja-JP"
link /dll /noentry /out:"$(OutDir)ja-JP\$(TargetFileName).mui" "$(IntermediateOutputPath)muiexample.ja-JP.res"
muirct -c "$(TargetPath)" -e "$(OutDir)ja-JP\$(TargetFileName).mui"
```

This is more complicated. We first compile the ja-JP RC file using mostly the same arguments as the en-US one, but one thing is different. Since we are building the ja-JP RC separately rather than with the language-neutral resources, we specify a `/fo` argument to output a dummy language-neutral RES file to be deleted later.

We then do the same directory creation and linking as before, but there is another command. The call to `muirct` simply takes the checksums from the main executable and puts it in the ja-JP MUI file. A non-matching checksum will render the MUI file unusable, and in this case, compiling the ja-JP resources separately has led to different checksums. However, `muirct` circumvents this issue.

```cmd
del "$(IntermediateOutputPath)temp.res"
```

This just deletes the dummy RES file we compiled. There's nothing to say here other than this only needs to be called once at the end of the post-build script, as compiling another RC file to this location will simply overwrite it with no warnings whatsoever.