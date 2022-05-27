# **ANNIE_Analysis**
 
## **Directory Naming Convention**
`<version of ANNIE geometry>_<top volume>_<number of identical runs>_<events per run>_<other run specific information>/`

Im sure there are better convensions I could have used but this was basically the first think I thought of. I'll probably change it later.

## **Scripts Usage**

### gstHist.C
**From Terminal**

`root -l </path/to>/Scripts/gstHist.C\(\{\"<path/to/>gntp.<#>.gst.root\"\, ... ,\"<path/to/>/gntp.<#>.gst.root\"},\"<branch name>\",\"<target name>\",\"<options>\"\)`

Example:
`root -l ../Scripts/gstHist.C\(\{\"1/gntp.1.gst.root\"\,\"2/gntp.2.gst.root\",\"3/gntp.3.gst.root\"\,\"4/gntp.4.gst.root\"},\"nfn\",\"\",\"P\"\)`
