#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

/*
 * 這個程式為了產生 1 ~ 99999999 的模擬數字，或是亂數，
 * 以送入 prizematch 程式做測試用的。
 * 用法：
 *
 *   $ geneNo  			直接執行，不加參數，則產生 1~99999999
 *   $ geneNo 5			產生5個亂數
 *
 *   結果會寫入 textNo.txt 檔案中
 *
 */
using namespace std;

int main(int argc, const char *argv[])
{
	char C;
	cout << "Usage:\n"
		<<  "\t" << argv[0] << " [num]\n"
		<<  "Option:\n"
		<<  "\t[num]\t生成給定數目的 8 位數亂數\n"
		<<  "\t\t若沒給定，則產生 1 ~ 99999999 的數字.\n"
		<<  "\t\t存於 textNo.txt\n"
		<< endl;

	ofstream ofs("testNo.txt");
	srand(time(NULL));
	int n=0;
	if (argc == 1) {
		cout	<<  "當程式一執行，會耗時數分鐘.\n"
				<<  "您確定要執行嗎？ (y/n) ";
		cin >> C;
		if ( C == 'y')
			for(; n<100000000; n++)
			ofs << n << endl;
	}
	else 
	{
		int limit = atoi(argv[1]);
		for (int i = 0; i < limit; i++) {
			n = rand()%100000000;
			ofs << n << endl;
			cout << n << endl;
		}
	}
	ofs.close();
	
	return 0;
}
