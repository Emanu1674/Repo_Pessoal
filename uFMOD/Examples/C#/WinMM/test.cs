// The following example shows how to build a standalone .NET application
// using uFMOD. Just set up your paths in build.bat and execute the batch.
// This example uses a CLI/C++ wrapper which makes it possible to
// combine managed and unmanaged code in a single executable. The complete
// source code is available in the cliwrapper subdirectory.

// Check api.htm for a complete uFMOD API reference.

class Program{
	static public void Main(){
		uFMOD.I_uFMOD.PlayRes(1, 0);
		System.Windows.Forms.MessageBox.Show("uFMOD ruleZ!", "C#");
		uFMOD.I_uFMOD.Stop();
	}
}