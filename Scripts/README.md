# Scripts Usage

## gstHist.C
### From Terminal

`$ root -l </path/to>/Scripts/gstHist.C\(\{\"<path/to/>gntp.<#>.gst.root\"\, ... ,\"<path/to/>/gntp.<#>.gst.root\"},\"<branch name>\",\"<target name>\",\"<options>\"\)`

Example:
`$ root -l ../Scripts/gstHist.C\(\{\"1/gntp.1.gst.root\",\"2/gntp.2.gst.root\",\"3/gntp.3.gst.root\",\"4/gntp.4.gst.root\"},\"nfn\",\"\",\"P\"\)`

### From ROOT

`root[#] .x </path/to>/Scripts/gstHist.C({"<path/to/>gntp.<#>.gst.root", ... ,"<path/to/>/gntp.<#>.gst.root"},"<branch name>","<target name>","<options>")`

Example:
`root[#] .x ../Scripts/gstHist.C({"1/gntp.1.gst.root","2/gntp.2.gst.root","3/gntp.3.gst.root","4/gntp.4.gst.root"},"nfn","","P")`