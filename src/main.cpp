#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stack>
#include <cstdlib>
#include <list>
#include <vector>
#include <cmath>
#include <sstream>

/*
 * 統一發票兌獎程式 V1.0  作者: Mingyi Wu  Date: 2012/5/7
 * V 1.1 加入手動對獎的功能
 *
 * 使用方法:
 *    $ ./prize Base.txt Owned.txt
 * Base.txt : 代表政府公佈的中獎號碼
 * Owned.txt : 代表自己輸入的統一發票號碼清單
 * 
 * ===========
 * Base.txt 的格式必須要按照以下格式:
 * 
 * 代號 數量
 * V 1.1 加入手動對獎的功能
 * 發票號碼
 *
 * 其中的代號需按照 S, F, H, A 的順序, 分別代表 特別獎S, 特獎F, 頭獎H, 增開獎A 的順序
 * 如果該月特別獎沒有號碼，則數量為0即可，但還是需要有 S 這代號。
 *
 * ex: 當期的頭獎有3個號碼
 * H 3
 * 11111111
 * 22222222
 * 33333333
 * 
 * ===========
 * Owned.txt 的格式為一列一個完整發票號碼. 電子發票的話，直接掃描條碼即可使用。
 * 但須包含前面完整的兩個英文字母，以供辨識。
 * 如果沒有英文字母，則抓取從開頭起算的8位數字視為發票號碼。
 *
*/
typedef	unsigned int uint;
using namespace std;

class PRIZE
{
	private:
	int digits; // 存放位數資料
	list<uint> base;
	string name;

	public:
	//PRIZE (int d)  : digits(d) { };
	void setdigits( const int d) { digits=d; };
	void add( const uint & );
	void addname( const string & text) { name = text; };
	void print();
	bool match(const uint &, string &, bool manually);
};

void PRIZE::add(const uint & data)
{
	base.push_back(data);
}

void PRIZE::print()
{
	list<uint>::iterator it;
	cout << "*" << name << " " << digits << "位數 *"<< endl; 
	for (it = base.begin(); it != base.end(); it++) {
		cout << *it << endl;
	}
}

// input: 欲對獎的發票號碼
// got  : 比對後的文字結果 (中獎 或 謝謝惠顧)
// manually : 是否為手動輸入資料? 如果是，
//            判斷特別獎(S), 特獎(F)的時候要特別小心，只要末三碼比對正確，就提示需要完整輸入!
bool PRIZE::match(const uint & input, string & got, bool manually = false)
{
	got.clear();
	got = "謝謝惠顧!";
	uint t_input = input % static_cast<uint> (pow(10, digits) );
	list<uint>::iterator it;
	for (it = base.begin(); it != base.end(); it++)
	{
		if ( (*it - t_input) == 0 )
		{
			got = "中獎！ 中" + name;
			return true;
		}
        // 手動輸入時, 可能只有輸入後3碼，因此如果後三碼與特別獎吻合時
        // 需額外提醒可能需要輸入全部號碼!以免錯失中特別獎。
        if ((name == "特獎" || name == "特別獎") && manually && ((*it % 1000) - t_input == 0)) 
        {
			got = "有機會! 末三碼與 " + name + " 吻合! 請輸入完整號碼!" ;
			return true;
        }
	}
	return false;
}

static void help(std::string argvName)
{
	using namespace std;
	cout << "################################\n";
	cout << "### 統一發票對獎程式 Ver 1.1 ###\n";
	cout << "################################\n";
	cout << "  使用方法:\n";
	cout << "    " << argvName << " [Option(s)] <BASE NUMBER> \n";
	cout << endl;
	cout << "      <Base Number>   政府公佈的中獎號碼"<< endl;
	cout << "      [Owned List]    欲對獎的統一發票號碼清單"<< endl;
	cout << "      [Option] 選項:"<< endl;
	cout << "           -f filename   輸入欲對獎的檔案(不使用手動輸入模式)\n";
	cout << "           -v            印出每張發票結果\n";
	cout << "                         (預設: 只顯示中獎發票.)"<< endl;
	cout << "           -h  印出此說明文件"<< endl;
	cout << endl;
	// cout << "           -m  手動對獎。此時 [Owned List filename] 可以省略"<< endl;
}

int main(int argc, const char *argv[])
{
	if (argc < 2) {
		help(argv[0]);
		return 1;
	}

	// baseFile 是 開獎號碼,
	// ownedFile 是 持有的發號碼票
	string baseFile;
	string ownedFile;
	bool verboseMode = false; // 預設不顯示每張發票號碼的得獎結果
	bool autoMode = false;  // 預設為手動輸入號碼

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];
		if ( arg == "-h" || arg == "--help" )
		{
			help(argv[0]);
			return 0;
		}
		else if (arg == "-v" )
		{
			verboseMode = true;
		}
		else if ( arg == "-f" )
		{
			if (i+1 < argc)  // 檢查參數夠不夠？
			{
				ownedFile = argv[++i];
				autoMode = true;
			}
			else
			{
				cerr << "-f 後方必須指定 檔案名稱." << endl;
				return 1;
			}
		}
		else
		{
			if ( baseFile.empty() )
				baseFile = arg;
			else
				return cerr << "錯誤! 輸入太多 Base file." << endl, -1;
		}
	}

	if (verboseMode) cout << "[Enter Verbose Mode] "<< endl;
	if (verboseMode && autoMode) cout << "[Enter Auto Mode] " << ownedFile << endl;
	
	fstream base(baseFile.c_str());
	if ( ! base.is_open() ) 
		return cerr << "Error when open base file."<< endl, -1;
	else if (verboseMode)
		cout << "Loaded base file: " << baseFile << endl;

	fstream data;
	if (autoMode)
	{
		data.open(ownedFile.c_str());
		if (! data.is_open() )
			return cerr << "Error(s) when open base file or owned file."<< endl, -1;
		else if (verboseMode)
			cout << "Loaded owned file: " << ownedFile << endl;
	}

	/* 讀入開獎號碼資料, 並建立對獎表格 PrizeList */
	list<uint> Base; // 開獎號碼
	stack<int> BaseSeq; //	號碼組數, 例: S:1, F:1, H:3, A:2
	string PrizeNo;	//號碼代號: S(特別獎), F(特獎), H(頭獎), A(增加獎)
	string tempStr;
	uint tempNo;
	char c;  // 偷窺下一行字首
	int times;
	do {
		c = base.peek();
		switch (c) 
		{
			default :
				getline(base, tempStr);
				break;

				// 特別獎，1組, 8位 2000萬
			case 'S':
			case 'F':
			case 'H':
			case 'A':
				base >> tempStr;
				PrizeNo = c + PrizeNo ;
				base >> times;
				BaseSeq.push(times);
				for (int i = 0; i < times; i++) {
					base >> tempNo;
					//tempStr.insert(0, 1, c);
					Base.push_back(tempNo);
					cout << c << " : " ;
					cout.width(8);
					cout << right << tempNo << endl;
				}
				break;
		}
	} while ( ! base.eof());
	
	base.close();

	/* 為了驗證資料用的 */
	if (PrizeNo != "AHFS") {
		cout << "輸入順序:" << PrizeNo << endl;
		return cerr<< "輸入資料錯誤！(順序不對) " << endl, -2;
	}

	/*
	cout << "輸入開獎號碼組數 (A, H, F, S) : " ;
	for (uint i =BaseSeq.size(); i>0 ;i--)
	{
		cout << BaseSeq.top() << ", ";
		BaseSeq.pop();
	}
	cout << endl;
	*/

	// Begin 讀入欲兌獎的發票號碼
	string one; // 紀錄一張發票號碼
	stack<uint> allD; // 存放所有發票資料
	/* 讀入欲對獎的發票資料 */
	if (autoMode)
	{
		getline(data,one);
		while ( ! data.eof() )
		{
			size_t found;
			if (one[0] != '#' )
			{
				// 尋找第一個不為數字的字母
				// 因為發票號碼可能為條碼輸入的資料, 可能為純數字
				// (條碼輸入的資料，會包含發票號碼的開頭2個英文字母)
				found = one.find_first_not_of("0123456789");
				if (found != string::npos) //找到英文字開頭
					one = one.substr(found+2, 8); // 擷取英文字後8個數字為發票號碼
				else
					one = one.substr(0, 8); //只不到英文字，直接紀錄8位數字
				allD.push(atoi(one.c_str()) );
			}
			data >> one;
		}
	}
	if (autoMode) data.close();
	// End   讀入欲兌獎的發票號碼

	vector<PRIZE> PrizeList; 	// 存放6,5,4,3,2,頭獎,特獎,特別獎共8種
	int num[]={3, 4, 5, 6, 7, 8, 8, 8};
	string Pname[]={ "六獎", "五獎", "四獎", "三獎", "二獎", "頭獎", "特獎", "特別獎" };
	for (int i = 0; i < 8; i++)
	{
		PRIZE oneprize;
        // 設定獎項位數
		oneprize.setdigits( num[i] );
		oneprize.addname( Pname[i] );
		if (i==0) {
			for( int j=BaseSeq.top(); j>0; j--)
			{
				// cout << "Base.back()=" << Base.back() << endl;
				oneprize.add( Base.back() );
				Base.pop_back();
			}
				BaseSeq.pop();
		}
		if (i==6) {
			Base.pop_back();
			Base.pop_back();
			Base.pop_back();
			BaseSeq.pop();
		}
		else if (i==7) {
			Base.pop_back();
			BaseSeq.pop();
		}

		//cout << "Base.back()=" << Base.back() << endl;
		list<uint>::iterator it=Base.end();
		it--;
		for (int j = BaseSeq.top() ;  j > 0 ; j--, it--)
	   	{
			//cout <<"*it... " <<  *it % static_cast<int>(pow(10, num[i]))  << endl;
			oneprize.add( *it % static_cast<int>(pow(10, num[i])) );
		}
		PrizeList.push_back(oneprize);
//		oneprize.clear();
	};

	/* 顯示各獎的對獎號碼 */
	if (verboseMode)
	{
		cout << "==================" << endl;
		cout << "顯示各獎的對獎號碼" << endl;
		cout << "==================" << endl;
		for (int i = 0; i < 8; i++) {
			PrizeList[i].print();
			
			cout << " ========" << endl;
		}
	}
	
	/* =======  進行兌獎 ======= */
	/* 
	 * 將發票號碼送入 PRIZE 的 match 程式比對。
	 * [autoMode]
	 * 輸入: 發票號碼, 獎別名稱字串
	 * 輸出: 是否中獎？  
	 *       預設: 有中獎才輸出，沒有中獎不提醒。
	 *       -v:   中獎的話，字串為獎別名稱，否則為 "謝謝惠顧!"
	 * [not autoMode]
	 * 輸入: 手動輸入一號碼. 若輸入 x , q 代表結束
	 * 輸出: 是否中獎？  
	 *       預設: 有中獎才輸出，沒有中獎不提醒。
	 *       -v:   中獎的話，字串為獎別名稱，否則為 "謝謝惠顧!"
	 */
	if (autoMode)
	{
		while(! allD.empty() )
		{
			string result;
			bool Rt = false;  // 是否中獎？
			if (verboseMode) cout << "比對發票 " << setw(8) << setfill('0') << allD.top() << " -> ";
			for (int i = 0; i < PrizeList.size(); i++)
			{
				if (PrizeList[i].match( allD.top(), tempStr , false))
				{
					result = tempStr; 
					Rt = true;
				}
			}
			if (Rt) 
				cout << "恭喜您 " << setw(8) << setfill('0') << allD.top() << " " << result << endl;
			else if (verboseMode)
				cout << tempStr << endl;
			allD.pop();
		}
	}
	else 
	{  // 手動輸入模式
		cout << "輸入發票號碼 (輸入 x 或 q 代表結束):" << endl;
		getline(cin, tempStr);

		// 將 tempStr 字串轉換為 stringStream, 再輸入 tempNo
		istringstream( tempStr ) >> tempNo;

		// cout << "tempStr=" << tempStr << endl;
		// cout << "tempNo=" << tempNo << endl;
		while( tempStr[0] != 'q' && tempStr[0] != 'x' )
		{
			bool Rt = false;  // 是否中獎？
			string result;
			for (int i = 0; i < PrizeList.size(); i++)
			{
				if (PrizeList[i].match( tempNo, tempStr, true ))
				{
					result = tempStr; 
					Rt = true;
				}
			}
			if (Rt) 
				cout << "恭喜您 " << setw(8) << setfill('0') <<tempNo << " " << result << endl;
			else
				cout << tempStr << endl;
			
			getline(cin, tempStr);
			istringstream( tempStr ) >> tempNo;
		}
	}
	return 0;
}
