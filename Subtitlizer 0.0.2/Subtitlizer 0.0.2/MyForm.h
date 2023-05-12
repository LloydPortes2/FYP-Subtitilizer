#pragma once
#include <iostream>
#include <string>
#include <windows.h>


namespace Subtitlizer002 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	// Function prototype
	BOOL SelectFile();
	// Declare a global variable to store the file path

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			
		}

		void RunGUI()
		{
			Application::Run(gcnew MyForm());
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ export_btn;
	private: System::Windows::Forms::Button^ select_btn;
	private: System::Windows::Forms::ProgressBar^ progressBar1;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::ComboBox^ input_lng_cb;

	private: System::Windows::Forms::ComboBox^ output_lng_cb;

	private: System::Windows::Forms::Label^ input_lng_lable;
	private: System::Windows::Forms::Label^ output_lng_lable;
	private: System::Windows::Forms::TextBox^ search_bar;
	private: System::Windows::Forms::VScrollBar^ vScrollBar1;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::ComponentModel::IContainer^ components;



	protected:


	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->export_btn = (gcnew System::Windows::Forms::Button());
			this->select_btn = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->vScrollBar1 = (gcnew System::Windows::Forms::VScrollBar());
			this->input_lng_cb = (gcnew System::Windows::Forms::ComboBox());
			this->output_lng_cb = (gcnew System::Windows::Forms::ComboBox());
			this->input_lng_lable = (gcnew System::Windows::Forms::Label());
			this->output_lng_lable = (gcnew System::Windows::Forms::Label());
			this->search_bar = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// export_btn
			// 
			this->export_btn->Location = System::Drawing::Point(745, 462);
			this->export_btn->Name = L"export_btn";
			this->export_btn->Size = System::Drawing::Size(75, 23);
			this->export_btn->TabIndex = 0;
			this->export_btn->Text = L"Export";
			this->export_btn->UseVisualStyleBackColor = true;
			this->export_btn->Click += gcnew System::EventHandler(this, &MyForm::export_btn_Click);
			// 
			// select_btn
			// 
			this->select_btn->Location = System::Drawing::Point(12, 21);
			this->select_btn->Name = L"select_btn";
			this->select_btn->Size = System::Drawing::Size(75, 23);
			this->select_btn->TabIndex = 1;
			this->select_btn->Text = L"Select File";
			this->select_btn->UseVisualStyleBackColor = true;
			this->select_btn->Click += gcnew System::EventHandler(this, &MyForm::select_btn_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(84, 496);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(667, 23);
			this->progressBar1->TabIndex = 2;
			this->progressBar1->Click += gcnew System::EventHandler(this, &MyForm::progressBar1_Click);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->vScrollBar1);
			this->panel1->Location = System::Drawing::Point(12, 64);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(674, 421);
			this->panel1->TabIndex = 3;
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Location = System::Drawing::Point(655, 14);
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(10, 393);
			this->vScrollBar1->TabIndex = 0;
			this->vScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &MyForm::vScrollBar1_Scroll);
			// 
			// input_lng_cb
			// 
			this->input_lng_cb->FormattingEnabled = true;
			this->input_lng_cb->Location = System::Drawing::Point(735, 149);
			this->input_lng_cb->Name = L"input_lng_cb";
			this->input_lng_cb->Size = System::Drawing::Size(121, 21);
			this->input_lng_cb->TabIndex = 4;
			// 
			// output_lng_cb
			// 
			this->output_lng_cb->FormattingEnabled = true;
			this->output_lng_cb->Location = System::Drawing::Point(735, 323);
			this->output_lng_cb->Name = L"output_lng_cb";
			this->output_lng_cb->Size = System::Drawing::Size(121, 21);
			this->output_lng_cb->TabIndex = 5;
			// 
			// input_lng_lable
			// 
			this->input_lng_lable->AutoSize = true;
			this->input_lng_lable->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->input_lng_lable->Location = System::Drawing::Point(742, 133);
			this->input_lng_lable->Name = L"input_lng_lable";
			this->input_lng_lable->Size = System::Drawing::Size(82, 13);
			this->input_lng_lable->TabIndex = 6;
			this->input_lng_lable->Text = L"Input Language";
			this->input_lng_lable->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// output_lng_lable
			// 
			this->output_lng_lable->AutoSize = true;
			this->output_lng_lable->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->output_lng_lable->Location = System::Drawing::Point(745, 307);
			this->output_lng_lable->Name = L"output_lng_lable";
			this->output_lng_lable->Size = System::Drawing::Size(90, 13);
			this->output_lng_lable->TabIndex = 7;
			this->output_lng_lable->Text = L"Output Language";
			// 
			// search_bar
			// 
			this->search_bar->Location = System::Drawing::Point(109, 23);
			this->search_bar->Name = L"search_bar";
			this->search_bar->Size = System::Drawing::Size(577, 20);
			this->search_bar->TabIndex = 8;
			this->search_bar->TextChanged += gcnew System::EventHandler(this, &MyForm::search_bar_TextChanged);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GrayText;
			this->ClientSize = System::Drawing::Size(868, 531);
			this->Controls->Add(this->search_bar);
			this->Controls->Add(this->output_lng_lable);
			this->Controls->Add(this->input_lng_lable);
			this->Controls->Add(this->output_lng_cb);
			this->Controls->Add(this->input_lng_cb);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->select_btn);
			this->Controls->Add(this->export_btn);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->panel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void vScrollBar1_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {
}
private: System::Void select_btn_Click(System::Object^ sender, System::EventArgs^ e) {
	//System::Diagnostics::Process::Start("explorer.exe");
	//openFileDialog.Filter = "Video Files|*.mp4;*.avi;*.wmv;*.mov;*.flv;*.mkv;*.webm";
	//openFileDialog.Title = "Select a video file";

	OPENFILENAME ofn;


	// a another memory buffer to contain the file name
	char szFile[100];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Video Files (*.avi;*.mp4;*.mkv)\0*.avi;*.mp4;*.mkv\0All Files (*.*)\0*.*\0";

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetOpenFileName(&ofn);

	// Display the file selection dialog
	if (GetOpenFileName(&ofn) == TRUE)
	{
		// The user selected a file, so get the file path


		// Do something with the selected file 
		
	}

	delete[] ofn.lpstrFile;
	
	}
private: System::Void search_bar_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	// Access the file path stored in the global variable

}
private: System::Void export_btn_Click(System::Object^ sender, System::EventArgs^ e) {
	timer1->Start(); //for prototype purposes
}
private: System::Void progressBar1_Click(System::Object^ sender, System::EventArgs^ e) {

}
private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
	progressBar1->Increment(1);
	if (progressBar1->Value == 100)//prototype purposes
	{
		timer1->Stop();
		
		progressBar1->Value = 0;
	}
}
};
}
