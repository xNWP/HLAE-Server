/*

	file: main.cpp
	author: Brett Anthony

	Main entry point to program, starts the main gui form.

	Comments throughout this project will be minimal as it's pretty straightforward, for
	a proper tutorial check on the github page as I will be posting one there (if it is not already posted).

*/


#pragma once
#include "MainForm.h"

int main()
{
	HLAEServerExample::MainForm^ GUI = gcnew HLAEServerExample::MainForm;
	GUI->ShowDialog();
	return 0;
}