#include "file.h"

void File::CreateFolder(string folderPath)
{
	string command;
	//command = "mkdir -p " + folderPath;//创建上层目录
	command = "mkdir " + folderPath;
	system(command.c_str());

}

void File::CreatePath()
{
	if (!(bool)PathIsDirectory("save"))
		CreateFolder("\"save\"");
	//else
	//	cout << "save Already Exists" << endl;
	if (!(bool)PathIsDirectory("screenshot"))
		CreateFolder("\"screenshot\"");
	//else
	//	cout << "screenshot Already Exists" << endl;
}

bool File::ExistTxt(string pathName)
{
	struct stat buffer;
	return (stat(pathName.c_str(), &buffer) == 0);
}

void File::WriteCube(int x, int y, int z, BlockType type)
{
	string content = to_string(x) + " " + to_string(y) + " " + to_string(z) + " " + to_string(type);
	outfile << content << endl;
}

void File::RemoveCube(int x, int y, int z)
{
	string content = to_string(x) + " " + to_string(y) + " " + to_string(z) + " -1";
	outfile << content << endl;
}

void File::Init()
{
	vx.clear();
	vy.clear();
	vz.clear();
	vtype.clear();
	CreatePath();
	outfile.close();
	cout << "Enter your file's name: " << endl;
	cin >> filename;
	if (ExistTxt("save\\\\" + filename + ".txt")) {//若文件已存在
		ReadFile();
	}
	else {//若文件不存在，创建
		bRead = false;
		cout << "Create a file successfully! " << endl;
		outfile.open("save\\\\" + filename + ".txt");
	}
}

void File::ReadFile()
{
	bRead = true;
	cout << "Load file successfully! " << endl;
	outfile.open("save\\\\" + filename + ".txt", ios::app);

	infile.open("save\\\\" + filename + ".txt", ios::in);
	if (!infile.is_open()) {
		cout << "Something went wrong when opening files" << endl;
	}
	else {
		while (!infile.eof()) {
			getline(infile, line);
			vector<string>parameters;
			string tailMark = " ";
			string ans = "";

			line = line.append(tailMark);
			for (int i = 0; i < line.length(); i++) {
				char ch = line[i];
				if (ch != ' ') {
					ans += ch;
				}
				else {
					parameters.push_back(ans);
					ans = "";
				}
			}
			if (parameters.size() != 4) {
				//cout << "the size is not correct" << endl;
			}
			else {
				for (int i = 0; i <= 3; i++) {
					int num = stoi(parameters[i].c_str());
					switch (i) {
					case 0:vx.push_back(num); break;
					case 1:vy.push_back(num); break;
					case 2:vz.push_back(num); break;
					case 3:vtype.push_back(num); break;
					}
				}
			}
		}
	}

	infile.close();
}