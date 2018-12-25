統一發票對獎程式 Ver 1.1
========================

安裝方法
--------
- Requirement: 
  1. GNU C++ compilier  (第1次使用後即可不用)
  2. Python3   (用來每2個月擷取開獎資訊用)

- In the Linux:
    $ make


使用方式
--------
1. 執行 getBase2.py 抓取最新的資料庫
   (最好上網確認一下號碼都沒錯, 因網頁排版方式可能改變)

2. 執行 prizematch 程式, 手動輸入
   ```$ prizmatch Base_YYY年MM-MM月.txt```

   或自動比對
   ```$ priamztch -f <發票資料檔> Base_YYY年MM-MM月.txt```

   `<發票資料檔>` 的內容通常是由 barcode scanner 掃描發票上的條碼存下的，
   而不是手動輸入的。


測試用檔案
----------
- geneNo.c 是用來產生測試 prizematch 用的檔案, debug 程式用。


版本紀錄
--------
- 2016/9/28
  完成 `getBase2.py`
  使用 `BeautifulSoup4` 處理下載下來的 HTML 檔案。方便且較正確。

- 2016/??/??
  使用 Python 寫一個新的替代 Shell script。完成 `getBase.py`
  增開六獎的部份最容易出現問題
  檢查開獎數量是否符合? (ex: A 4 代表應該有 4 組號碼,
  如果沒有，修改 `Base.txt` 檔案, 將 A 4 改成符合的數量)

- 2015/9/5
  `getBase.old.sh` 是舊版 Shell script 程式，但因為不方便使用，
  改寫一個 Python 版本的，使用上較為方便。


免責聲明
--------
此程式是用來練習 C++ 語言所完成的小程式。不保證沒 bug.
若使用此程式對獎, 有錯失首獎或說中獎卻沒中的情況，
皆與此程式設計人無關，請自行負責。
