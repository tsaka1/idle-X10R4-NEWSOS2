# idle-X10R4-NEWSOS2
An old screen-saver program for X10R4 on NEWS-OS2.2

File name | Description
--------- | -----------
idle.Z | Archived file made on October 1990, tar & compress
idle.list | The file list of idle.Z
idle/Makefile | Makefile for old BSD make
idle/Makefile,v | Old RCS repository file of Makefile
idle/README | README file written in Japanese (Shift-JIS code)
idle/csr.icon | X10 bitmap image file of corsor
idle/csr.icon,v | Old RCS repository file of csr.icon
idle/csr_mask.icon | Bit mask pattern of csr.icon
idle/csr_mask.icon,v | Old RCS repository file of csr_mask.icon
idle/idle | Compiled binary for NEWS-OS2.2
idle/idle.c | Main source code written in C
idle/idle.c,v | Old RCS repository file of idle.c
idle/idle.icon | X10 bitmap image file of moving icon
idle/idle.icon,v | Old RCS repository file of idle.icon
idle/idle.man | On-line manual written in Japanese (Shift-JIS code)
idle/idle.man,v | Old RCS repository file of idle.man
idle/idle_mask.icon | Bit mask pattern of idle.icon
idle/idle_mask.icon,v | Old RCS repository file of idle_mask.icon
idle/mail | A mail message to msgs for announcement to users
idle/shar | A shar (SHell ARchive) file for distributing via fj.sources

## 思い出話
この idle は図書館情報大学の学生だった頃に研究室に導入されたUNIX Workstation
SONY NEWS (NWS-841) の標準のスクリーンセーバが画面がいきなり真っ赤になったり
真っ青になったりするという心臓に悪いものだったので、作成したものです。

当時 Macintosh 用にフリーで配布されているスクリーンセーバの idle というものが
暗くした画面上を Macintosh のアイコンがひょこひょこと表示されるので、それと
同様の動きをするので、idle と名付けています。

当時のNEWS の OS (NEWS-OS2.2) は 4.2BSD ベースで日本語文字コードは Shift-JIS
であり、GUI は X-Window System Version 10 Release 4 (X10R4)ベースでした。
その環境でのみ動作確認し、他の機関でも NEWS を使っているところで欲しいという
声もあったので、NetNews の fj.sources で配布しました。残念ながら、fj.sources
に流した私の article は手元に残っていません。

なお、おまけの機能として GUI ではなく、当時はまだまだ活用されていた文字端末で
ログインした時用に、curses モード、つまり文字端末モードも備えています。文字端末
としては、CIT-600のような専用機よりは PC-98シリーズで端末エミュレータソフトを
使ったものが多くなってきた時期だったと思います(図書館情報大学の場合)。

これを配布してまもなく、NEWS-OSも2.x から 3.x に変わり、X-Window もVersion 11
が主流になってしまったので、既に役に立たないものとなっていますが、github で
改めて公開する次第です。これを現在の環境向けにアップデートすることはしませんが、
これをベースにした派生ソフトウェアの作成はしていただいて構いません。(その場合は
簡単にクレジットに言及していただければ幸いです。)

