Example 13.1 has been removed since the DirectMusic API has
been deprecated in the new versions of DirectX. Check out the
XAudio2 library (DX Documentation), or use an 3rd party sound 
library like FMOD (www.fmod.org).


In order to get the examples working that uses 
DirectShow you must download and install the 
Windows SDK (available at microsoft.com).

The Windows SDK contains the necessary library
and include files for DirectShow.

Remeber to link strmiids.lib to your project
as well as set the VC++ paths to the library &
include folder of the Windows SDK:

VS2005: Tools->Options->VC++ Directories

