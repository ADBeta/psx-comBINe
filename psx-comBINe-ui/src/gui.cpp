#include <iostream>

// Disable all the warnings JUST for wx-widgets
// Editor note: fix your shit wx. That many warnings is a joke
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
////
#include <wx/wx.h>
////
#pragma GCC diagnostic pop


class MainApp : public wxApp {
	public:
	virtual bool OnInit();
};


class MainFrame : public wxFrame {
	public:
	MainFrame();

	// Define the UI elements used on this frame
	wxStaticBoxSizer    *CueSelBox;
	wxButton            *CueSelBtn;
	wxStaticText        *CueSelTxt;

	wxStaticBoxSizer    *DirSelBox;
	wxButton            *DirSelBtn;
	wxStaticText        *DirSelTxt;

	wxButton        *QuitBtn;

	// Define the funtions and events used on this frame
	void OnAbout(wxCommandEvent &event);
	void OnSelectCue(wxCommandEvent &event);
	void OnSelectDir(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);

};


// Enumerate event IDs
enum {
	ID_QuitBtn       = 1,
	
	ID_CueSelBtn     = 2,
	ID_CueSelTxt     = 3,

	ID_DirSelBtn     = 4,
	ID_DirSelTxt     = 5,
};


// Eqivelant of main() for the GUI to run in background
wxIMPLEMENT_APP_NO_MAIN(MainApp);

// Window init function - opens the frame
bool MainApp::OnInit() {
	MainFrame *frame = new MainFrame();
	frame->Show(true);
	frame->SetMinSize(wxSize(550, 400));
	return true;
}


MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "psx-comBINe") {
	/*** Create Menu Bar *****************************************************/
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
 
	//TODO: remove file & quit 
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
 
	SetMenuBar(menuBar);

	// Set event bindings for menu objects
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);

	/*** Create and default status bar ***************************************/
	CreateStatusBar();
	// SetStatusText("Welcome to wxWidgets!");


	/*** Define UI Element Objects *******************************************/


	



	
	

	// Create Window Sizer Flags defaults TODO: Move this
	wxSizerFlags SizerMainDefault(1);
	SizerMainDefault.Border(wxALL, 10).CenterHorizontal();










	/** Cue file selection text and button **/
	CueSelBtn = new wxButton(this, ID_CueSelBtn, _T("Select"),
	                                       wxDefaultPosition, wxSize(100, 30));

	CueSelTxt = new wxStaticText(this, ID_CueSelTxt, _T("Select a File..."),
	                wxDefaultPosition, wxSize(400, 40), wxST_ELLIPSIZE_START);

	// Create a box for the cue selection UI
	CueSelBox = new wxStaticBoxSizer(wxHORIZONTAL, this, 
												      _T("Select .cue File") );

	CueSelBox->Add(CueSelBtn, wxSizerFlags().Border(wxALL, 5).Left() );
	CueSelBox->Add(CueSelTxt, wxSizerFlags().Border(wxALL, 5).Left() );
	



	/** Output Directory Selection text and button **/
	DirSelBtn = new wxButton(this, ID_DirSelBtn, _T("Select"),
	                                       wxDefaultPosition, wxSize(100, 30));
	DirSelBtn->Enable(false);

	DirSelTxt = new wxStaticText(this, ID_DirSelTxt, _T("..."),
	                wxDefaultPosition, wxSize(400, 40), wxST_ELLIPSIZE_START);

	// Create a box for the cue selection UI
	DirSelBox = new wxStaticBoxSizer(wxHORIZONTAL, this, 
											   _T("Select Output Directory") );

	DirSelBox->Add(DirSelBtn, wxSizerFlags().Border(wxALL, 5).Left() );
	DirSelBox->Add(DirSelTxt, wxSizerFlags().Border(wxALL, 5).Left() );







	// Define a grid sizer then add the UI Elements to it.
	wxGridSizer *uiGrid = new wxGridSizer(1, 10, 10);
	
	uiGrid->Add(CueSelBox, SizerMainDefault);
	uiGrid->Add(DirSelBox, SizerMainDefault);


	// Add the quit button to the bottom right of the window
	QuitBtn = new wxButton(this, ID_QuitBtn, _T("Quit"));
	uiGrid->Add(QuitBtn, SizerMainDefault.Bottom().Right());
	SetSizer(uiGrid);








	/*** UI Element Event Bindings *******************************************/
	Bind(wxEVT_BUTTON, &MainFrame::OnSelectCue, this, ID_CueSelBtn);
	Bind(wxEVT_BUTTON, &MainFrame::OnSelectDir, this, ID_DirSelBtn);
	Bind(wxEVT_BUTTON, &MainFrame::OnExit, this, ID_QuitBtn);
}





/*** Event Callback Functions ************************************************/
// Exit button or menu item closes the GUI and ends the program
void MainFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

// Prints the About Message
void MainFrame::OnAbout(wxCommandEvent &event) {
	//TODO: Fix this to be centered
	const wxString about = 
"psx-comBINe Version 5\n\
PSX Multi-BIN (.bin/.cue) Game file combiner\n\
(c) ADBeta 2024";

	wxMessageBox(about, _T("About psxComBINe"), wxOK | wxCENTRE);
}

// Open a file dialog to select a .cue file
void MainFrame::OnSelectCue(wxCommandEvent &event) {
	wxFileDialog cueFileDialog(this, _T("Open Cue file"), "", "", 
                 "CUE files (*.cue)|*.cue", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	
	// If the user closes the dialog, exit the event
	if(cueFileDialog.ShowModal() == wxID_CANCEL) return;

	// If the user chose a file, Set the CueSelTxt string to the selected
	// filename, enable the directory button and set the output directory
	// string to the parent of the selected file + psx-comBINe
	std::string cueFilename = static_cast<std::string>(cueFileDialog.GetPath());

	this->CueSelTxt->SetLabel(cueFilename);
	this->DirSelTxt->SetLabel(cueFilename + ".output");

	this->DirSelBtn->Enable(true);


	// Update the layout for both boxes
	this->CueSelBox->Layout();
	this->DirSelBox->Layout();
}

// Open a file dialog to select only a directory to output to
void MainFrame::OnSelectDir(wxCommandEvent &event) {
	
	wxDirDialog dirDialog(this, _T("Select Output Director"), "",
	                                 wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	
	// If the user closes the dialog, exit the event
	if(dirDialog.ShowModal() == wxID_CANCEL) return;

	// If user selected a directory, set the text to reflect that
	this->DirSelTxt->SetLabel(dirDialog.GetPath());

	// Update the layout of the text
	this->DirSelBox->Layout();

}
