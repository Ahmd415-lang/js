#include <iostream> 
#include <iomanip>
#include <string>
#include <cmath>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <vector>
#include <fstream>
#include "json.hpp"
using namespace std;
using namespace std::chrono;
using json = nlohmann::json;
template <class T>
void addItem (vector <T>& vec, T& item) {
	vec.push_back(item);
}
///////////////////////////////////////////////////////////
class Logout : public exception {
public:
	virtual const char* what() const throw() override {
		return ("Logged out...");
	}
};
///////////////////////////////////////////////////////////
class Question {
public:
	virtual void to_json(json& j) const {
		j["index"] = index;
		j["Qtext"] = Qtext;
		j["score"] = score;
	}
	void SetIndex(int i) {
		this->index=i;
	}
	void SetNext() {
		next = NULL;
	}
	virtual void ShowQ() {
		cout<<"Error!"<<endl;
	}
	virtual string ShowA() {
		cout << "Error!"<< endl;
		return "";
	}
	void SetText() {
		cout << "Question text : " << endl;
		string Qtext;
		getline(cin >> ws,  Qtext);
		this->Qtext = Qtext;
	}
	void SetText(int i) {
		Qtext = " ";
	}
	void SetScore(int i) {
		score = i;
	}
	void SetScore() {
		cout<<"Score of this question: ";
		bool first=true;
		while(true) {
			if(!first) {
				cout<<"Try again: ";
			}
			string scoreString;
			getline(cin >> ws, scoreString);
			float s = stof(scoreString);
			if(s<0) {
				cout << "Scores cannot be less than 0!" <<endl;
				first = false;
				continue;
			}
			if(fmod(s, 0.25) == 0.0f) {
				score = s;
				cout<<endl;
				break;
			}
			else {
				cout<<"Scores must be in inrements of 0.25"<<endl;
				first = false;
			}
		}
	}
	float GetScore() {
		return score;
	}
	virtual float GetNScore() {
		return 0;
	}
	int Getindex() {
		return index;
	}
	string Gettext() {
		return Qtext;
	}
	virtual void SetAnswer() {
		cout<<"Error!"<<endl;
	}
	virtual void SetNscore() {
		cout<<"Error!"<<endl;
	}
	virtual bool CheckAnswer() {
		return false;
	}
	virtual bool isDescribtive() {
		return false;
	}
protected:
	string Qtext;
	int index;
	float score;
	Question* next;
	friend class Exam;
};
/////////////////////////////////////////////////////////////////////////////
class Q4choice : public Question{
public:
	Q4choice(int i) {
		SetIndex(i);
		SetNext();
		SetText();
		SetAnswer();
		SetScore();
		SetNscore();
		correctChoice=1;
	}
	Q4choice(const json& j) {
		Qtext = j.at("Qtext").get<string>();
		index = j.at("index").get<int>();
		score = j.at("score").get<float>();
		negativeScore = j.at("negativeScore").get<float>();
		array<string, 4> tmp = j.at("choices").get<array<string, 4>>(); // Fix: Use array<string, 4>
		copy(tmp.begin(), tmp.end(), choices);
	}
	virtual void to_json(json& j) const override {
        Question::to_json(j);
        j["type"] = "Q4choice";
        j["choices"] = choices;
        j["negativeScore"] = negativeScore;
    }
	virtual void ShowQ() override {
		cout<<Getindex()<<") "<<Gettext()<<"?"<<endl;
		string choicestmp[4];
		for(int i=0; i<4; i++)
			choicestmp[i]=choices[i];
		srand(time(0));
		for(int i=4; i>0; i--){
			int index = rand() % i;
			char c = 4-i+'A';
			cout<<c<<") "<<choicestmp[index]<<endl;
			if(index!=3) {
				for(int j=index; j<3; j++)
					swap(choicestmp[j], choicestmp[j+1]);
			}
		}
		for(int i=0; i<4; i++) {
			if(choicestmp[i]==choices[0]){
				correctChoice=i+'A';
				break;
			}
		}
	}
	virtual string ShowA() override{
		cout<<"Correct answer is: "<<choices[0]<<endl;
		return choices[0];
	}
	virtual void SetAnswer() override {
		for(int i=0; i<4; i++){
			char c = 'A'+i;
			cout<<c<<". ";
			if(i==0)
				cout<<"(Correct choice) ";
			string ch;
			getline(cin >> ws, ch);
			choices[i]=ch;
		}
	}
	virtual float GetNScore() override {
		return negativeScore;
	}
	virtual void SetNscore() override {
		cout<<"Negative score for this question: ";
		bool first=true;
		while(true) {
			if(!first) {
				cout<<"Try again: ";
			}
			string scoreString;
			getline(cin >> ws, scoreString);
			float s = stof(scoreString);
			if(s<0) {
				cout << "Scores cannot be less than 0!" <<endl;
				first = false;
				continue;
			}
			if(fmod(s, 0.25) == 0.0f) {
				negativeScore = s;
				cout<<endl;
				break;
			}
			else {
				cout<<"Scores must be in inrements of 0.25"<<endl;
				first = false;
			}
		}
	}
	virtual bool CheckAnswer() override{
		string choice;
		while(true) {
			getline(cin >> ws, choice);
			if(choice == "A" || choice == "B" || choice == "C" || choice == "D")
				break;
			else
				cout << "Invalid input! Try again." <<endl;
		}
		char c = choice[0];
		if(c==correctChoice)
			return true;
		else
			return false;
	}
	virtual bool isDescribtive() override{
		return false;
	}
private:
	float negativeScore;
	string choices[4];
	char correctChoice;
};
//////////////////////////////////////////////////////////////////
class Qshort : public Question{
public:
	Qshort(int i) {
		SetIndex(i);
		SetNext();
		SetText();
		SetAnswer();
		SetScore();
		SetNscore();
	}
	Qshort(const json& j) {
		Qtext = j.at("Qtext").get<string>();
		index = j.at("index").get<int>();
		score = j.at("score").get<float>();
		negativeScore = j.at("negativeScore").get<float>();
		answer = j.at("answer").get<string>();
	}
	virtual void to_json(json& j) const override {
        Question::to_json(j);
        j["type"] = "Qshort";
        j["answer"] = answer;
        j["negativeScore"] = negativeScore;
    }
	virtual void ShowQ() override{
		cout<<Getindex()<<") "<<Gettext()<<"?"<<endl;
	}
	virtual string ShowA() override{
		cout<<"Correct answer is: "<<answer<<endl;
		return answer;
	}
	virtual void SetAnswer() override {
		string a;
		while(true) {
			cout<<"Answer: ";
			getline(cin >> ws,  a);
			if(a.find(' ')!=string::npos) 
				cerr<<"Answer must be only a word or a number."<<endl;
			else
				break;
		}
		answer=a;
	}
	virtual float GetNScore() override {
		return negativeScore;
	}
	virtual void SetNscore() override {
		cout<<"Negative score for this question: ";
		bool first=true;
		while(true) {
			if(!first) {
				cout<<"Try again: ";
			}
			string scoreString;
			getline(cin >> ws, scoreString);
			float s = stof(scoreString);
			if(s<0) {
				cout << "Scores cannot be less than 0!" <<endl;
				first = false;
				continue;
			}
			if(fmod(s, 0.25) == 0.0f) {
				negativeScore = s;
				cout<<endl;
				break;
			}
			else {
				cout<<"Scores must be in inrements of 0.25"<<endl;
				first = false;
			}
		}
	}
	virtual bool CheckAnswer() override {
		string a;
		getline(cin >> ws,  a);
		if(a==answer)
			return true;
		else
			return false;
	}
	virtual bool isDescribtive() override{
		return false;
	}
private:
	float negativeScore;
	string answer;
};
/////////////////////////////////////////////////////////////////////////
class Qessay : public Question {
public:
	Qessay() {
		SetIndex(0);
		SetNext();
		SetText(0);
		answer = "";
		SetScore(0);
		solve = "";
	}
	Qessay(int i) {
		SetIndex(i);
		SetNext();
		SetText();
		SetAnswer();
		SetScore();
		solve = "";
	}
	Qessay(const json& j) {
		Qtext = j.at("Qtext").get<string>();
		index = j.at("index").get<int>();
		score = j.at("score").get<float>();
		answer = j.at("answer").get<string>();
		solve = j.at("solve").get<string>();
	}
	virtual void to_json(json& j) const override {
        Question::to_json(j);
        j["type"] = "Qessay";
        j["answer"] = answer;
        j["solve"] = solve;
    }
	virtual void ShowQ() override{
		cout<<Getindex()<<") "<<Gettext()<<"?"<<endl;
	}
	virtual string ShowA() override{
		cout<<"Correct answer is: "<<endl;
		cout<<answer<<endl;
		return answer;
	}
	virtual void SetAnswer() override {
		cout<<"Answer (Enter an empty line to finish): "<<endl;
		string line;
		while(getline(cin, line)) {
			if(line.empty())
				break;
			answer+=line+"\n";
		}	
	}
	virtual float GetNScore() override {
		return 0;
	}
	virtual bool CheckAnswer() override {
		cout << "Press Enter twice to finish." <<endl;
		string input, line;
		solve = "";
		while(getline(cin, line)) {
			if(line.empty()) {
				if (!input.empty())
					break;
				continue;
			}
			input+=line+"\n";
		}
		solve = input;
		return false;
	}
	virtual bool isDescribtive() override{
		return true;
	}
	string GetSolve() {
		return solve;
	}
private:
	string answer;
	string solve;
};
///////////////////////////////////////////////////////////////////
class AnswerSheet {
public:
	AnswerSheet() {
		ID="";
		name[0]=name[1]="";
		questions.clear();
		ClaimedScore = 0;
		marked = false;
	}
	AnswerSheet(const string& id, const string* name, float otherScores) {
		ID=id;
		this->name[0]=name[0];
		this->name[1]=name[1];
		questions.clear();
		ClaimedScore = otherScores;
		marked = false;
	}
	AnswerSheet(const json& j) {
		ID = j.at("ID").get<string>();
		ClaimedScore = j.at("ClaimedScore").get<float>();
		array<string, 2> tmp = j.at("name").get<array<string, 2>>();
		copy(tmp.begin(), tmp.end(), name);
		marked = j.at("marked").get<bool>();
		for (const auto& essayjson : j.at("questions")) {
			questions.emplace_back(essayjson);
		}
	}
	void to_json(json& j) const {
        j["ID"] = ID;
        j["name"] = {name[0], name[1]};
        j["ClaimedScore"] = ClaimedScore;
        j["marked"] = marked;
        json Questions;
        for (const auto& q : questions) {
            json question;
            q.to_json(question);
    		Questions.push_back(question);
        }
		j["questions"] = Questions;
    }
	void SetClaimedScore(float s) {
		ClaimedScore=s;
	}
	void AddAns(Qessay Q) {
		addItem(questions, Q);
	}
	bool CheckIfMarked() {
		return marked;
	}
	string GetID() {
		return ID;
	}
	void overview() {
		cout << name[0] <<" " <<name[1] << " | " <<ID <<endl;
	}
	void view() {
		for(int i=0; i<questions.size(); i++) {
			cout<<"Student: ";
			for(int i=0; i<2; i++)
				cout<<name[i]<<" ";
			cout<<endl;
			cout<<"ID: "<<ID<<endl<<endl;
			cout<<questions[i].Getindex()<<") "<<questions[i].GetSolve()<<endl;
			cout<<"Enter V to view the suggested answer."<<endl;
			cout<<"Enter Q to view the question text."<<endl;
			cout<<"Enter S to score this question."<<endl;
			string input;
			getline(cin >> ws, input);
			if(input=="V" || input=="v") {
				questions[i].ShowA();
				i--;
				cout<<"Enter any button to exit."<<endl;
				string x;
				getline(cin >> ws, x);
			}
			else if(input=="Q" || input=="q") {
				questions[i].ShowQ();
				i--;
				cout<<"Enter any button to exit."<<endl;
				string x;
				getline(cin >> ws, x);
			}
			else if(input=="S" || input=="s") {
				cout<<"Score from "<<questions[i].GetScore()<<" : ";
				bool first = true;
				while(true){
					if(!first)
						cout<<"Try again: ";
					string scoreString;
					getline(cin >> ws, scoreString);
					float s = stof(scoreString);
					if(s<0 || s>questions[i].GetScore()) {
						cout << "Score must be between 0 and " << questions[i].GetScore()<<endl;
						first = false;
						continue;
					}
					else if(fmod(s, 0.25) != 0.0f) {
						cout<<"Scores must be in inrements of 0.25"<<endl;
						first = false;
						continue;
					}
					ClaimedScore+=s;
					break;
				}
			}
		}
		marked = true;
	}
	float GetClaimedScore() {
		return ClaimedScore;
	}
	string GetName() {
		string n = name[0]+" "+name[1];
		return n;
	}
private:
	vector<Qessay> questions;
	string name[2];
	string ID;
	float ClaimedScore;
	bool marked;
};
//////////////////////////////////////////////////////////////////
class Exam {
public:
	Exam() {
		TeacherID = "";
		name = "";
		NumQ = 0;
		ETime=0;
		nonDescribtives = 0;
		Score = 0;
		answerSheets.clear();
		questions.clear();
		scores.clear();
	}
	Exam(string name, string id) {
		TeacherID = id;
		this->name = name;
		int i;
		srand(time(0));
		for (i=0; i<4; i++)
			unicode[i] = 97 + rand() % 26;
		for (i=4; i<10; i++)
			unicode[i] = 48 + rand() % 10;
		unicode[10]= '\0';
		NumQ = 0;
		ETime=0;
		nonDescribtives = 0;
		Score = 0;
		answerSheets.clear();
		questions.clear();
		scores.clear();
	}
	Exam(const json& j) {
		name = j.at("name").get<string>();
		TeacherID = j.at("TeacherID").get<string>();
		strcpy(unicode, j.at("unicode").get<string>().c_str());
		NumQ = j.at("NumQ").get<int>();
		ETime = j.at("ETime").get<int>();
		Score = j.at("Score").get<float>();
		nonDescribtives = j.at("nonDescribtives").get<float>();
		questions.clear();
		for (const auto& qjson : j.at("questions")) {
			Question* newQ = nullptr;
			string type = qjson.at("type").get<string>();
			if (type == "Qshort")
				newQ = new Qshort(qjson);
			else if (type == "Q4choice")
				newQ = new Q4choice(qjson);
			else if (type == "Qessay")
				newQ = new Qessay(qjson);
			questions.push_back(newQ);
		}
		for (const auto& sheetjson : j.at("answerSheets")) {
			AnswerSheet sheet(sheetjson);
			answerSheets.emplace_back(sheet);
		}
		if(j.contains("scores"))
			scores = j.at("scores").get<vector<float>>();
	}
	void to_json(json& j) {
		j["name"] = name;
		j["TeacherID"] = TeacherID;
        j["unicode"] = unicode;
        j["NumQ"] = NumQ;
        j["ETime"] = ETime;
        j["Score"] = Score;
        j["nonDescribtives"] = nonDescribtives;
		json Questionsjson;
		for (const auto& q : questions) {
			json questionjson;
			q->to_json(questionjson);
			Questionsjson.push_back(questionjson);
		}
		j["questions"] = Questionsjson;
		json Answersheets;
        for (const auto& sheet : answerSheets) {
            json sheetjson;
            sheet.to_json(sheetjson);
            Answersheets.push_back(sheetjson);
        }
		j["answerSheets"] = Answersheets;
		j["scores"] = scores;
	}
	void SetTime(int t) {
		ETime=t;
	}
	string GetUnicode() {
		return unicode;
	}
	string GetName() {
		return name;
	}
	int GetNumQ() {
		return NumQ;
	}
	string GetID() {
		return TeacherID;
	}
	Question* find(int i) {
		if (i < 1 || i > questions.size())
			return nullptr;
		return questions[i - 1];
	}
	void AddQ(Question* q) {
		questions.push_back(q);
		q->SetIndex(questions.size());
		cout << "Added successfully!" << endl;
		Score += q->score;
	}
	void AddND(float n) {
		nonDescribtives+=n;
	}
	void AddAnswerSheet(AnswerSheet& sheet) {
		addItem(answerSheets, sheet);
	}
	void RemoveQ(int i) {
		if (i < 1 || i > questions.size()) {
			cout << "Index out of range!" << endl;
			return;
		}
		delete questions[i - 1];
		questions.erase(questions.begin() + (i - 1));
		for (int j = i - 1; j < questions.size(); j++) {
			questions[j]->SetIndex(j + 1);
		}
		cout << "Question " << i << " has been removed." << endl;
	}
	void ShowE() {
		cout << name << endl;
		for (const auto& q : questions) {
			q->ShowQ();
			q->ShowA();
		}
	}
	void Participation(const string& ID, const string* name) {
	    cout << "Time of the exam : " << ETime << " minute(s)" << endl;
	    cout << "Enter any button to begin." << endl;
	    string z;
	    getline(cin >> ws, z);
	    auto startTime = steady_clock::now();
	    float StuScore = 0;
	    AnswerSheet studentAnsSheet(ID, name, 0);
	    vector<int> wrongs;
	    atomic<bool> rt(false);
	    atomic<bool> finish(false);
	    thread timer([&rt, &finish, this]() {
	        auto start = steady_clock::now();
	        while (duration_cast<minutes>(steady_clock::now() - start).count() < ETime && !finish) {
	            this_thread::sleep_for(seconds(1));
	        }
	        if (!finish) {
	            rt = true;
	            cout << "\nTime is up!" << endl;
	            cout << "Enter any button to exit" << endl;
	        }
	    });
	    cout << "Exam has begun." << endl;
	    for (auto& t : questions) {
	        if (rt)
	            break;
	        t->ShowQ();
	        if (t->isDescribtive()) {
	            Qessay* Q = dynamic_cast<Qessay*>(t);
	            if (Q) {
	                Q->CheckAnswer();
	                studentAnsSheet.AddAns(*Q);
	            }
	        } else if (t->CheckAnswer())
	            StuScore += t->score;
	        else {
	            StuScore -= t->GetNScore();
	            wrongs.push_back(t->index);
	        }
	    }
	    finish = true;
	    timer.join();
	    if (nonDescribtives > 0) {
	        studentAnsSheet.SetClaimedScore(StuScore);
	        float score = 20.0 * StuScore / nonDescribtives;
	        cout << "Your score: " << fixed << setprecision(2) << score << endl;
	        cout << "!!!This score is calculated without the essay section!!!" << endl;
	    } else
	        cout << "Your answers have been recorded." << endl;
	    AddAnswerSheet(studentAnsSheet);
	    scores.push_back(StuScore);
	    sort(scores.begin(), scores.end(), greater<float>());
	    ofstream fout("file.txt");
	    fout << "Score earned from 4CQs and short-answer questions: " << StuScore << endl;
	    fout << "Incorrect responses: See the right answers below." << endl << endl;
	    for (const auto& i : wrongs) {
	        fout << i << ") correct answer is:" << endl;
	        fout << questions[i - 1]->ShowA() << endl;
	    }
	    fout << "Average score for this exam: " << average() << endl;
	    fout << "Max score for this exam: " << scores[0] << endl;
	    fout << "Your rank in this exam: " << rank(StuScore);
	    fout.close();
	    cout << "Your report card is ready!" << endl;
	    cout << "After the marking, you can view your final score in your exams menu." << endl;
	}
	int findAnsSheet(string id) {
		for(int i=0; i<answerSheets.size(); i++) {
			if(answerSheets[i].GetID() == id)
				return i;
		}
		return -1;
	}
	float SeeScore(string id) {
		int index = findAnsSheet(id);
		if(index == -1)
			return -100;
		if(!answerSheets[index].CheckIfMarked())
			return -100;
		else {
			float finalScore = 20.0*answerSheets[index].GetClaimedScore()/Score;
			return finalScore;
		}
	}
	void Mark() {
		if (answerSheets.empty()) {
			cout << "There have been no exam participants yet!" <<endl;
			return;
		}
		while (true) {
			for(int i=0; i<answerSheets.size(); i++) {
				cout <<i+1<<". ";
				answerSheets[i].overview();
			}
			cout << "Enter the student ID whose exam you want to mark (or B to return to main menu): ";
			string id;
			getline (cin >> ws, id);
			if(id == "B" || id== "b")
				return;
			int index = findAnsSheet(id);
			if(index == -1) {
				cout << "ID not found!" << endl;
				continue;
			}
			AnswerSheet& student = answerSheets[index];
			if(student.CheckIfMarked()) {
				cout << "Marked already!" <<endl;
				continue;;
			}
			student.view();
			cout << "Enter A to mark another student." <<endl;
			getline(cin >> ws, id);
			if(id == "N" || id == "n")
				return;
		}
	}
	float average() {
		float S=0;
		for(const auto& score : scores) 
			S+=score;
		return S/scores.size();
	}
	int rank(float score) {
		for(int i=0; i<scores.size(); i++) {
			if(scores[i]==score)
				return i+1;
		}
	}
	void SortAnswerSheetsByScore() {
		for(int i=0; i<answerSheets.size(); i++) {
			for(int j=i; j<answerSheets.size(); j++) {
				if(answerSheets[i].GetClaimedScore()<answerSheets[j].GetClaimedScore())
					swap(answerSheets[i], answerSheets[j]);
			}
		}
	}
	void ScoresList() {
		ofstream fout("file.txt");
		SortAnswerSheetsByScore();
		float S=0;
		for(auto& sheet : answerSheets) {
			fout << sheet.GetName() <<"---->"<<sheet.GetClaimedScore()<<"/"<<Score<<endl;
			S+=sheet.GetClaimedScore(); 
		}
		float avg = S/answerSheets.size();
		fout << "Average score for this exam: " << avg <<endl;
		fout << "Max score for this exam: " << answerSheets[0].GetClaimedScore() << endl;
		fout.close();
	}
private:
	vector<Question*> questions;
	string TeacherID;
	char unicode[11];
	string name;
	int NumQ;
	int ETime;
	float Score;
	float nonDescribtives;
	vector<AnswerSheet> answerSheets;
	vector<float> scores;
};
/////////////////////////////////////////////////////////////////////////////
class TS {
public:
	bool isaccessible() {
		return accessible;
	}
	void Setaccessible() {
		accessible = true;
	}
	void ban() {
		accessible = false;
	}
	void SetName(string Fname, string Lname) {
		for (char c : Fname) {
			if (!((c>='a' && c<='z')||(c>='A' && c<='Z')))
				throw invalid_argument("First/Last name must contain only letters!");
		}
		for (char c : Lname) {
			if (!((c>='a' && c<='z')||(c>='A' && c<='Z')))
				throw invalid_argument("First/Last name must contain only letters!");
		}
		name[0] = Fname;
		name[1] = Lname;
	}
	void SetID(string ID) {
		if (ID == "O" || ID == "o")
			throw Logout();
		if (ID.length() != 9)
			throw invalid_argument("ID must be exactly 9 characters long!");
		for (char c : ID) {
			if (c < '0' || c > '9') {
				throw invalid_argument("ID must contain only digits!");
			}
		}
		this->ID = ID;
	}
	void SetPass(string password) {
		if (password == "O" || password == "o")
			throw Logout();
		if(password.length()<4 || password.length()>8)
			throw invalid_argument("Password must be at least 4 characters long and at most 8!");
		bool hasDigit=false;
		bool hasLcase=false;
		bool hasUcase=false;
		for(char c : password) {
			if(c >= '0' && c <= '9')
				hasDigit=true;
			else if(c >= 'a' && c <= 'z')
				hasLcase=true;
			else if(c >= 'A' && c <= 'Z')
				hasUcase=true;
			else {
				throw invalid_argument("Invalid character detected!");
			}
		}
		if(!hasDigit || !hasUcase || !hasLcase)
			throw invalid_argument("password must containat least one digit/Uppercase letter/lowercase letter!");
		this->password = password;
	}
	string GetID() {
		return ID;
	}
	string* GetName() {
		return name;
	}
	string GetPass() {
		return password;
	}
	virtual void ShowInf() {
		cout<<"Error!"<<endl;
	}
	virtual void menu() {
		cout<<"Error!"<<endl;
	}
	virtual void to_json(json& j) const {  
        j["ID"] = ID;
        j["name"] = {name[0], name[1]};
        j["password"] = password;
		j["Myexams"] = Myexams;
		j["accessible"] = accessible;
    }
	virtual void from_json(const json& j) {
		array<string, 2> tmp = j.at("name").get<array<string, 2>>();
		copy(tmp.begin(), tmp.end(), name);
		ID = j.at("ID").get<string>();
		password = j.at("password").get<string>();
		Myexams = j.at("Myexams").get<vector<string>>();
		accessible = j.at("accessible").get<bool>();
	}
protected:
	vector <string> Myexams;
private:
	string ID;
	string name[2];
	string password;
	bool accessible;
};
///////////////////////////////////////////////////////////////////////////////////
class Users {
public:
	Users() {
		LTS.clear();
		madeExams.clear();
	}
	void SaveToFile(const string& file_Users, const string& file_Exams);
	void LoadFromFile(const string& file_Users, const string& file_Exams);
	void AddTS(TS* newTS) {
			addItem(LTS, newTS);
			cout << "Signed up successfully!" << endl;
			newTS->menu();
	}
	void AddExam(Exam e) {
		addItem(madeExams, e);
	}
	void Login () {
		int i;
		bool ValidID = false;
		while(!ValidID) {
			cout << "Enter your ID:" << endl;
			string id;
			getline(cin >> ws, id);
			i = CheckIfExistTS(id);
			if (i == 0) {
				cout << "Invalid ID! Please try again." << endl;
				continue;
			}
			else if(i == -1) {
				cout << "This account has been blocked." <<endl;
				cout << "Please contact support to resolve the issue" <<endl;
				return;
			}
			ValidID = true;
			int attempts = 3;
			while (attempts > 0) {
				cout << "Enter your password (" << attempts << " attempts left):" << endl;
				string pass;
				getline(cin >> ws, pass);
				if (CheckPass(pass, i)) {
					break;
				}
				attempts--;
				if (attempts == 0) {
					for(auto& user : LTS ) {
						if(user->GetID()==id) {
							user->ban();
						}
					}
					cout << "Too many incorrect attempts. Your account has been blocked!!!" << endl;
					return;
				}
			}
		}
	}
	inline int CheckIfExistTS(string id) {
		if(id == "O" || id == "o")
			throw Logout();
		if (LTS.size() == 0)
			return 0;
		for (int i = 0; i < LTS.size(); i++) {
			if (LTS[i]->GetID() == id) {
				if(LTS[i]->isaccessible())
					return i + 1;
				else
					return -1;
			}
		}
		return 0;
	}
	int CheckPass(string pass, int index) {
		if(pass == "O" || pass == "o")
			throw Logout();
		if (LTS[index - 1]->GetPass() == pass) {
			cout << "Logged in successfully." << endl << "Welcome!" << endl;
			LTS[index - 1]->menu();
			return 1;
		} else {
			cout << "Incorrect password!" << endl;
			return 0;
		}
	}
	int GetNoExams() {
		return madeExams.size();
	}
	Exam* findExam(string code) {
		for(int i=0 ; i<madeExams.size(); i++) {
			if(madeExams[i].GetUnicode() == code)
				return &madeExams[i];
		}
	}
	vector<Exam>& GetMadeExams() {
		return madeExams;
	}
private:
	vector<TS*> LTS;
	vector<Exam> madeExams;
};
Users users;
/////////////////////////////////////////////////////////////////////////////
class Teacher : public TS {
public:
	void AddCourse(string c) {
		if (c == "O" || c == "o")
			throw Logout();
		addItem(courses, c);
		cout << "Added successfully!" << endl;
	}
	void AddExam(Exam e) {
		users.AddExam(e);
		Myexams.push_back(e.GetUnicode());
		cout << "New exam Added!" << endl;
		cout << "Exam code is: " << e.GetUnicode() <<endl;
	}
	virtual void to_json(json& j) const override {
        TS::to_json(j);  
        j["type"] = "Teacher";
        j["courses"] = courses; 
    }
	virtual void from_json(const json& j) override {
		TS::from_json(j);
		courses = j.at("courses").get<vector<string>>();
	}
	Teacher(const json& j) {
		from_json(j);
	}
	Teacher() {
		courses.clear();
		Setaccessible();
		bool Validname=false;
		do {
			cout << "First name:" << endl;
			string Fname;
			getline(cin >> ws, Fname);
			if(Fname == "O" || Fname == "o")
				throw Logout();
			cout << "Last name:" << endl;
			string Lname;
			getline(cin >> ws, Lname);
			if(Lname == "O" || Lname == "o")
				throw Logout();
			try {
				SetName(Fname, Lname);
				Validname=true;
			} catch (invalid_argument& e) {
				cout << "Error : " << e.what() <<endl;
			}
		} while(!Validname);
		cout << "Enter the courses name one by one:" << endl << "!Enter F to finish!" << endl;
		while (true) {
			string c;
			getline(cin >> ws, c);
			if (c == "F" || c == "f")
				break;
			else
				AddCourse(c);
		}
		string id;
		bool first = false;
		do {
			if (first)
				cout << "Enter a different ID." << endl;
			cout << "ID (must be 9 digits):" << endl;
			getline(cin >> ws, id);
			try {
				SetID(id);
				if (users.CheckIfExistTS(id))
					throw invalid_argument("ID already exists!");
			} catch (invalid_argument& e) {
				cout << "Error: " << e.what() << endl;
				first = true;
				continue;
			}
			break;
		} while (true);
		string password;
		bool ValidPass = false;
		do {
			cout << "Choose a password (Password should be a mix of Upper/Lower case letters and digits):" << endl;
			getline(cin >> ws, password);
			try {
				SetPass(password);
				ValidPass=true;
			} catch (invalid_argument& e) {
				cout<< "Error :"<< e.what()<<endl;
			}
		} while(!ValidPass);
	}
	virtual void ShowInf() override {
		cout << "Name:" << GetName()[0] <<" "<<GetName()[1] << endl;
		cout << "Course(s):" << endl;
		for (int i = 0; i < courses.size(); i++) {
			cout << courses[i] << endl;
		}
		cout << "ID:" << GetID() << endl;
	}
	int NomyExams() {
		int n = 0;
		for (int i=0; i<users.GetMadeExams().size(); i++) {
			if(users.GetMadeExams()[i].GetID()==GetID())
				n++;
		}
		return n;
	}
	void ShowExamsList() {
		for (int i=0; i<Myexams.size(); i++) {
			cout << i+1 <<". " << Myexams[i]<< endl;
		}
	}
	virtual void menu() override {
		while (true) {
			ShowInf();
			cout << "Enter V to view your exams." << endl;
			cout << "Enter C to create a new exam." << endl;
			cout << "Enter M to mark exams."<<endl;
			string input1;
			getline(cin >> ws, input1);
			if (input1 == "C" || input1 == "c") {
				cout << "Exam name: " << endl;
				string n;
				getline(cin >> ws, n);
				Exam newExam(n, GetID());
				cout << "Exam created!" << endl << "New exam code is: " << newExam.GetUnicode() << endl;
				while (true) {
					cout << "Enter A to add a question." << endl;
					cout << "Enter R to remove a question." << endl;
					cout << "Enter F to finish." << endl;
					string input2;
					getline(cin >> ws, input2);
					if (input2 == "A" || input2 == "a") {
						cout<<"Enter 4 if you want to create a 4choice question."<<endl;
						cout<<"Enter S for creating a short answer question"<<endl;
						cout<<"Enter E for creating an essay-type question"<<endl;
						string input3;
						getline(cin >> ws, input3);
						if(input3=="4") {
							Question* newQ = new Q4choice(newExam.GetNumQ()+1);
							newExam.AddQ(newQ);
							newExam.AddND(newQ->GetScore());
						}
						else if(input3=="S" || input3=="s") {
							Question* newQ = new Qshort(newExam.GetNumQ()+1);
							newExam.AddQ(newQ);
							newExam.AddND(newQ->GetScore());
						}
						else if(input3=="E" || input3=="e") {
							Question* newQ = new Qessay(newExam.GetNumQ()+1);
							newExam.AddQ(newQ);
						}
					}
					else if (input2 == "R" || input2 == "r") {
						if (newExam.GetNumQ()==0)
							cout << "No questions yet!" << endl;
						else {
							newExam.ShowE();
							cout << "Number of the question you want to remove : " << endl;
							string index;
							getline(cin >> ws, index);
							int i = stoi(index);
							newExam.RemoveQ(i);
						}
					}
					else if (input2 == "F" || input2 == "f") {
					    cout << "Time of the exam (minute) : " << endl;
						string time;
						getline(cin >> ws, time);
						int t = stoi(time);
						newExam.SetTime(t);
						AddExam(newExam);
						break;
					}
				}
			}
			else if (input1 == "V" || input1 == "v") {
				if(!NomyExams())
					cout << "No exams yet!" << endl;
				else {
					ShowExamsList();
					cout << "Number of the exam you want to view : " << endl;
					string tmp;
					getline(cin >> ws, tmp);
					int i = stoi(tmp);
					if(i>NomyExams() || i<1)
						cout<<"Index out of range!" <<endl;
					else {
						Exam* e = users.findExam(Myexams[i-1]);
						e->ShowE();
						while (true) {
							cout << "Enter S to see scores list for this exam." <<endl;
							cout << "Enter B to button to exit." << endl;
							string input2;
							getline(cin >> ws, input2);
							if(input2=="B" || input2=="b") 
								break;
							else if(input2=="S" || input2=="s") {
								e->ScoresList();
								cout << "Scores list is ready!" <<endl;
							}
							else {
								cout << "Invalid input!" <<endl;
								continue;
							}
						}
					}
				}
			}
			else if (input1 == "M" || input1 == "m") {
				if(Myexams.size()==0) {
					cout << "No exams yet!" << endl;
					continue;
				}
				ShowExamsList();
				cout << "Number of the exam you want to view : " << endl;
				string number;
				getline(cin >> ws, number);
				int n = stoi(number);
				if(n>Myexams.size() || n<1)
					cout<<"Index out of range!" <<endl;
				else {
					Exam* e = users.findExam(Myexams[n-1]);
					e->Mark();
				}
			}
			else if (input1 == "O" || input1 == "o")
				return;
			else 
				cout << "Invalid command!" <<endl;
		}
	}
private:
	vector<string> courses;
};
/////////////////////////////////////////////////////////////////////////////////
class Student : public TS {
public:
	Student() {
		Myexams.clear();
		Setaccessible();
		bool Validname=false;
		do {
			cout << "First name:" << endl;
			string Fname;
			getline(cin >> ws, Fname);
			if(Fname == "O" || Fname == "o")
				throw Logout();
			cout << "Last name:" << endl;
			string Lname;
			getline(cin >> ws, Lname);
			if(Lname == "O" || Lname == "o")
				throw Logout();
			try {
				SetName(Fname, Lname);
				Validname=true;
			} catch (invalid_argument& e) {
				cout << "Error : " << e.what() <<endl;
			}
		} while(!Validname);
		cout << "Your Field of Study:" << endl;
		getline(cin >> ws, field);
		if(field == "O" || field == "o")
			throw Logout();
		string id;
		bool first = false;
		do {
			if (first)
				cout << "Enter a different ID." << endl;
			cout << "ID (must be 9 digits):" << endl;
			getline(cin >> ws, id);
			try {
				SetID(id);
				if (users.CheckIfExistTS(id))
					throw invalid_argument("ID already exists!");
			} catch (invalid_argument& e) {
				cout << "Error: " << e.what() << endl;
				first = true;
				continue;
			}
			break;
		} while (true);
		string password;
		bool ValidPass = false;
		do {
			cout << "Choose a password (Password should be a mix of Upper/Lower case letters and digits):" << endl;
			getline(cin >> ws,password);
			try {
				SetPass(password);
				ValidPass=true;
			} catch (invalid_argument& e) {
				cout<< "Error :"<< e.what()<<endl;
			}
		} while(!ValidPass);
	}
	virtual void to_json(json& j) const override {
        TS::to_json(j);
        j["type"] = "Student";
        j["field"] = field; 
    }
	virtual void from_json(const json& j) override {
		TS::from_json(j);
		field = j.at("field").get<string>();
	}
	Student(const json& j) {
		from_json(j);
	}
	virtual void ShowInf() override {
		cout << "Name:" << GetName()[0] <<" " << GetName()[1] << endl;
		cout << "Field of Study:" << field << endl;
		cout << "ID:" << GetID() << endl;
	}
	void ShowExamList() {
		for (int i=0; i<Myexams.size(); i++) {
			cout << i+1 << ". " << Myexams[i]<< " || ";
			Exam* e = users.findExam(Myexams[i]);
			if(e->SeeScore(GetID())==-100) 
				cout<<"Final score pending submission"<<endl;
			else {
				float finalScore = e->SeeScore(GetID());
				cout << fixed << setprecision(2) <<finalScore <<"/20"<<endl;
			}
		}
	}
	bool CheckNewExam(string code) {
		for(const auto& examcode : Myexams) {
			if(examcode == code)
				return false;
		}
		return true;
	}
	virtual void menu() override {
		while (true) {
			ShowInf();
            cout << "Enter R to register for an exam." << endl;
			cout << "Enter V to view exams you registered for."  << endl;
			cout << "Enter P to participate in an exam." << endl;
			string input;
			getline(cin >> ws,input);
			if (input == "R" || input == "r") {
				string code;
				cout << "The exam code (you need to get it from your teacher) : " << endl;
				getline(cin >> ws,code);
				bool found = false;
				for (int i=0; i<users.GetNoExams(); i++) {
					if (users.GetMadeExams()[i].GetUnicode() == code) {
						found = true;
						if(CheckNewExam(code)) {
							Myexams.push_back(code);
							cout << "Registered for " << users.GetMadeExams()[i].GetName() << " exam successfully!" << endl;
							break;
						}
						else {
							cout << "Registered for this exam already!" <<endl;
							break;
						}
					}
				}
				if(!found)
				    cout << "Invalid code!" << endl;
			}
			else if (input == "V" || input == "v") {
				if (!Myexams.size())
				    cout << "You have not registered for any exam yet!" << endl;
				else {
					ShowExamList();   
					cout << "Enter any button to exit." << endl;
					string x;
					getline(cin>>ws,x);
				}
			}
			else if (input == "P" || input == "p") {
				if (!Myexams.size()) {
				    cout << "You have not registered for any exam yet!" << endl;
					continue;
				}
				cout << "Code of the exam you want to participate in : " << endl;
				string code;
				getline(cin>>ws, code);
				bool found;
				for (int i=0; i<Myexams.size(); i++) {
					if (Myexams[i] == code){
						Exam* e = users.findExam(code);
						e->Participation(GetID(), GetName());
						found = true;
						break;
					}
				}
				if(!found)
				    cout << "Invalid code!" << endl;
			}
			else if(input == "O" || input == "o")
			    return;
		}
	}
private:
	string field;
};
//////////////////////////////////////////////////////////////////
void Users::LoadFromFile(const string& file_Users, const string& file_Exams) {
    ifstream fin_LTS(file_Users);
    if (!fin_LTS)
        return;
    json j_LTS;
    fin_LTS >> j_LTS;
    fin_LTS.close();
	for (const auto& userjson : j_LTS) {
		if (userjson.contains("type")) {
			string type = userjson.at("type").get<string>();
			if (type == "Teacher") {
				TS* t = new Teacher(userjson);
				LTS.push_back(t);
			} else if (type == "Student") {
				TS* s = new Student(userjson);
				LTS.push_back(s);
			}
		}
	}
    ifstream fin_Exams(file_Exams);
    if (!fin_Exams)
        return;
    json j_Exams;
    fin_Exams >> j_Exams;
    fin_Exams.close();
	for (const auto& examjson : j_Exams)
		madeExams.push_back(Exam(examjson));
}
void Users::SaveToFile(const string& file_Users, const string& file_Exams) {
	json json_Users, json_Exams;
	for (const auto& ts : LTS) {
		json Users_data;
		ts->to_json(Users_data);
		json_Users.push_back(Users_data);
	}
	for (auto& exam : madeExams) {
		json Exams_data;
		exam.to_json(Exams_data);
		json_Exams.push_back(Exams_data);
	}
	ofstream fout_LTS(file_Users);
	fout_LTS << json_Users.dump(4);
	fout_LTS.close();
	ofstream fout_Exams(file_Exams);
	fout_Exams << json_Exams.dump(4);
	fout_Exams.close();
}
///////////////////////////////////////////////////////////////////
int main() {
	users.LoadFromFile("Users_data.json", "Exams_data.json");
	while (true) {
		cout << "If you are a new user, Enter S, to sign up." << endl;
		cout << "If you signed up before, Enter L, to log in." << endl;
		cout << "Enter O whenever you want to log out."<< endl;
		cout << "Enter E to exit." << endl;
		string input1;
		getline(cin >> ws, input1) ;
		if (input1 == "s" || input1 == "S") {
			cout << "If you want to sign up as a Teacher, Enter T." << endl;
			cout << "If you want to sign up as a Student, Enter S." << endl;
			string input2;
			try {
				getline(cin>>ws, input2);
				if(input2 == "O" || input2 == "o")
					throw Logout();
				if (input2 == "T" || input2 == "t") {
					TS* newT = new Teacher();
					users.AddTS(newT);
				}
				else if (input2 == "S" || input2 == "s") {
					TS* newS = new Student();
					users.AddTS(newS);
				}
			} catch (Logout& e) {
				cout << e.what() << endl;
			}
		}
		else if (input1 == "L" || input1 == "l") {
			try {
				users.Login();
			} catch (Logout& e) {
				cout << e.what() <<endl;
			}
		}
		else if (input1 == "E" || input1 == "e") {
			users.SaveToFile("Users_data.json", "Exams_data.json");
			cout << "Exiting program..." << endl;
			break;
		}
	}
	return 0;
}