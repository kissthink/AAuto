
leptonica官网
http://www.leptonica.com

tesseract官网
http://code.google.com/p/tesseract-ocr/

可以参考网上的教程自行训练样本，
或者打开下面的下载地址下载官方提供的语言样本，
http://code.google.com/p/tesseract-ocr/downloads/list
或者直接安装tesseract-ocr官网提供的安装程序，最后打开 C:\Program Files\Tesseract-OCR\tessdata 直接复制官方提供的样本。

下载或生成 *.traineddata 样本文件放到"\lib\tesseract\.res\tessdata"目录下
也可以在导入tesseract支持库以前使用 win.setenv("TESSDATA_PREFIX","tessdata目录的上级目录")自定义样本目录。


