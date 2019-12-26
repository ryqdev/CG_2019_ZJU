#include "file.h"

void File::CreateFolder(string folderPath)
{
	string command;
	//command = "mkdir -p " + folderPath;//创建上层目录
	command = "mkdir " + folderPath;
	system(command.c_str());

}

bool File::ExistTxt(string pathName)
{
	struct stat buffer;
	return (stat(pathName.c_str(), &buffer) == 0);
}

void File::Init()
{
	outfile.close();
	cout << "Enter your file's name: " << endl;
	cin >> filename;
	while (ExistTxt("save\\\\" + filename + ".txt")) {
		cout << "File already exists" << endl;
		cout << "Enter your file's name: " << endl;
		cin >> filename;
	}
	cout << "Create a file successfully! " << endl;
	CreateFolder("\"save\"");
	outfile.open("save\\\\" + filename + ".txt");
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

void File::ReadFile()
{
	string line;
	fstream infile;

	cout << "Enter your file's name: " << endl;
	cin >> openfilename;
	while (!ExistTxt("save\\\\" + openfilename + ".txt")) {
		cout << "File not exists" << endl;
		cout << "Enter your file's name: " << endl;
		cin >> openfilename;
	}
	cout << "Load file successfully! " << endl;
	outfile.close();
	outfile.open("save\\\\" + openfilename + ".txt", ios::app);

	infile.open("save\\\\" + openfilename + ".txt", ios::in);
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