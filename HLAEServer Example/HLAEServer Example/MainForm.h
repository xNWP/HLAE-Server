#pragma once
#include "ServerControl.h"

namespace HLAEServerExample {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			UI_MANAGER->RunWorkerAsync();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected:
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  lblVer;
	private: System::Windows::Forms::Label^  lblMap;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  lblXpos;
	private: System::Windows::Forms::Label^  lblYpos;
	private: System::Windows::Forms::Label^  lblZpos;
	private: System::Windows::Forms::Label^  lblXrot;
	private: System::Windows::Forms::Label^  lblYrot;
	private: System::Windows::Forms::Label^  lblZrot;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  lblTime;
	private: System::Windows::Forms::Label^  lblFov;

	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label11;
	private: System::ComponentModel::BackgroundWorker^  UI_MANAGER;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->lblVer = (gcnew System::Windows::Forms::Label());
			this->lblMap = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->lblXpos = (gcnew System::Windows::Forms::Label());
			this->lblYpos = (gcnew System::Windows::Forms::Label());
			this->lblZpos = (gcnew System::Windows::Forms::Label());
			this->lblXrot = (gcnew System::Windows::Forms::Label());
			this->lblYrot = (gcnew System::Windows::Forms::Label());
			this->lblZrot = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->lblTime = (gcnew System::Windows::Forms::Label());
			this->lblFov = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->UI_MANAGER = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(121, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(45, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Version:";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(135, 39);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(31, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Map:";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// lblVer
			// 
			this->lblVer->AutoSize = true;
			this->lblVer->Location = System::Drawing::Point(163, 26);
			this->lblVer->Name = L"lblVer";
			this->lblVer->Size = System::Drawing::Size(33, 13);
			this->lblVer->TabIndex = 0;
			this->lblVer->Text = L"blank";
			this->lblVer->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// lblMap
			// 
			this->lblMap->AutoSize = true;
			this->lblMap->Location = System::Drawing::Point(163, 39);
			this->lblMap->Name = L"lblMap";
			this->lblMap->Size = System::Drawing::Size(33, 13);
			this->lblMap->TabIndex = 0;
			this->lblMap->Text = L"blank";
			this->lblMap->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(54, 69);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(57, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"X-Position:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(54, 82);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(57, 13);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Y-Position:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(54, 95);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(57, 13);
			this->label5->TabIndex = 1;
			this->label5->Text = L"Z-Position:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(189, 69);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(60, 13);
			this->label6->TabIndex = 1;
			this->label6->Text = L"X-Rotation:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(189, 82);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(60, 13);
			this->label7->TabIndex = 1;
			this->label7->Text = L"Y-Rotation:";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(189, 95);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(60, 13);
			this->label8->TabIndex = 1;
			this->label8->Text = L"Z-Rotation:";
			// 
			// lblXpos
			// 
			this->lblXpos->AutoSize = true;
			this->lblXpos->Location = System::Drawing::Point(108, 69);
			this->lblXpos->Name = L"lblXpos";
			this->lblXpos->Size = System::Drawing::Size(33, 13);
			this->lblXpos->TabIndex = 2;
			this->lblXpos->Text = L"blank";
			// 
			// lblYpos
			// 
			this->lblYpos->AutoSize = true;
			this->lblYpos->Location = System::Drawing::Point(108, 82);
			this->lblYpos->Name = L"lblYpos";
			this->lblYpos->Size = System::Drawing::Size(33, 13);
			this->lblYpos->TabIndex = 2;
			this->lblYpos->Text = L"blank";
			// 
			// lblZpos
			// 
			this->lblZpos->AutoSize = true;
			this->lblZpos->Location = System::Drawing::Point(108, 95);
			this->lblZpos->Name = L"lblZpos";
			this->lblZpos->Size = System::Drawing::Size(33, 13);
			this->lblZpos->TabIndex = 2;
			this->lblZpos->Text = L"blank";
			// 
			// lblXrot
			// 
			this->lblXrot->AutoSize = true;
			this->lblXrot->Location = System::Drawing::Point(246, 69);
			this->lblXrot->Name = L"lblXrot";
			this->lblXrot->Size = System::Drawing::Size(33, 13);
			this->lblXrot->TabIndex = 2;
			this->lblXrot->Text = L"blank";
			// 
			// lblYrot
			// 
			this->lblYrot->AutoSize = true;
			this->lblYrot->Location = System::Drawing::Point(246, 82);
			this->lblYrot->Name = L"lblYrot";
			this->lblYrot->Size = System::Drawing::Size(33, 13);
			this->lblYrot->TabIndex = 2;
			this->lblYrot->Text = L"blank";
			// 
			// lblZrot
			// 
			this->lblZrot->AutoSize = true;
			this->lblZrot->Location = System::Drawing::Point(246, 95);
			this->lblZrot->Name = L"lblZrot";
			this->lblZrot->Size = System::Drawing::Size(33, 13);
			this->lblZrot->TabIndex = 2;
			this->lblZrot->Text = L"blank";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(133, 122);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(33, 13);
			this->label9->TabIndex = 3;
			this->label9->Text = L"Time:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(137, 135);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(29, 13);
			this->label10->TabIndex = 3;
			this->label10->Text = L"FoV:";
			// 
			// lblTime
			// 
			this->lblTime->AutoSize = true;
			this->lblTime->Location = System::Drawing::Point(163, 122);
			this->lblTime->Name = L"lblTime";
			this->lblTime->Size = System::Drawing::Size(33, 13);
			this->lblTime->TabIndex = 2;
			this->lblTime->Text = L"blank";
			// 
			// lblFov
			// 
			this->lblFov->AutoSize = true;
			this->lblFov->Location = System::Drawing::Point(163, 135);
			this->lblFov->Name = L"lblFov";
			this->lblFov->Size = System::Drawing::Size(33, 13);
			this->lblFov->TabIndex = 2;
			this->lblFov->Text = L"blank";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(15, 182);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(341, 75);
			this->textBox1->TabIndex = 5;
			this->textBox1->Text = L"mirv_pgl url \"ws://127.0.0.1:30000/\"\r\nmirv_pgl start\r\nmirv_pgl dataStart";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(15, 163);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(282, 13);
			this->label11->TabIndex = 6;
			this->label11->Text = L"Type the following commands in csgo to send data to app:";
			// 
			// UI_MANAGER
			// 
			this->UI_MANAGER->WorkerReportsProgress = true;
			this->UI_MANAGER->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainForm::UI_MANAGER_DoWork);
			this->UI_MANAGER->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MainForm::UI_MANAGER_ProgressChanged);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(368, 271);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->lblFov);
			this->Controls->Add(this->lblZrot);
			this->Controls->Add(this->lblZpos);
			this->Controls->Add(this->lblTime);
			this->Controls->Add(this->lblYrot);
			this->Controls->Add(this->lblYpos);
			this->Controls->Add(this->lblXrot);
			this->Controls->Add(this->lblXpos);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->lblMap);
			this->Controls->Add(this->lblVer);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"HLAEServer Sample Application";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		private: ServerControl^ ServerManagement;

#pragma endregion
	private: System::Void UI_MANAGER_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
	{
		ServerManagement = gcnew ServerControl;
		// Updates ui to values
		while (1 < 2)
		{
			ServerManagement->UPDATE_DATA();
			UI_MANAGER->ReportProgress(0);
		}
	}
private: System::Void UI_MANAGER_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e)
{
	lblVer->Text = ServerManagement->client_version;
	lblMap->Text = ServerManagement->client_map;
	lblTime->Text = ServerManagement->client_time;
	lblFov->Text = ServerManagement->client_fov;
	lblXpos->Text = ServerManagement->client_xpos;
	lblYpos->Text = ServerManagement->client_ypos;
	lblZpos->Text = ServerManagement->client_zpos;
	lblXrot->Text = ServerManagement->client_xrot;
	lblYrot->Text = ServerManagement->client_yrot;
	lblZrot->Text = ServerManagement->client_zrot;
}
};
}
