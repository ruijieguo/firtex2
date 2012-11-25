doc_rootdir = $(top_srcdir)/documentation

DOC_SOURCE = $(doc_rootdir)/src/user_manual/zh-cn/user_manual.xml
CSS_FILE = $(doc_rootdir)/docbook/css/stylesheet.css
XSL_FILE = $(doc_rootdir)/docbook/xsl/mydocbook.xsl
DOCBOOK_ROOT=$(doc_rootdir)/docbook/docbook-xsl-ns-1.76.1
FO_XSL_FILE = $(DOCBOOK_ROOT)/fo/docbook.xsl
HTML_TARGET = $(doc_rootdir)/src/html/user_manual.html
PDF_TARGET = $(doc_rootdir)/src/pdf/user_manual.pdf
HTML_ARG=--stringparam section.autolabel 1 \
	--stringparam  section.label.includes.component.label 1 \
	--stringparam chunker.output.encoding UTF-8 \
	--stringparam admon.graphics 1 \
	--stringparam use.id.as.filename 1 \
	--stringparam navig.graphics 1 \
	--stringparam navig.graphics.extension .png\
	--stringparam html.stylesheet css/stylesheet.css

FOP_COMPILER = fop
FO_TARGET = $(doc_rootdir)/src/pdf/user_manual.fo
PDF_BODY_FONT=FZFangSong
PDF_MONOSPACE_FONT=FZFangSong
PDF_DINGBAT_FONT=FZFangSong
PDF_TITLE_FONT=FZFangSong

FO_ARG=--stringparam use.extensions 0 \
	--stringparam fop1.extensions 1 \
	--stringparam l10n.gentext.language zh_cn \
	--stringparam hyphenate false \
	--stringparam body.font.family $(PDF_BODY_FONT) \
	--stringparam monospace.font.family $(PDF_MONOSPACE_FONT) \
	--stringparam dingbat.font.family $(PDF_DINGBAT_FONT) \
	--stringparam title.font.family $(PDF_TITLE_FONT) \
	--stringparam paper.type A4 

FOP_CONF=$(doc_rootdir)/fop/conf/userconfig.xml

docbook:
	make cleandoc
	unzip -d $(doc_rootdir)/docbook/ $(doc_rootdir)/docbook/docbook-xsl-ns-1.76.1.zip
	mkdir html
	xsltproc --output $(HTML_TARGET) $(HTML_ARG) $(XSL_FILE) $(DOC_SOURCE)
	cp -r $(DOCBOOK_ROOT)/images html/
	cp -r $(doc_rootdir)/docbook/css html/
	xsltproc --output $(FO_TARGET) $(FO_ARG) $(FO_XSL_FILE) $(DOC_SOURCE)
	$(FOP_COMPILER) -c $(FOP_CONF) $(FO_TARGET) $(PDF_TARGET)

cleandoc:
	rm -rf $(doc_rootdir)/docbook/docbook-xsl-ns-1.76.1
	rm -rf html
	rm -rf pdf

EXTRA_DIST = \
	user_manual/zh-cn/introduction.xml\
	user_manual/zh-cn/query_syntax.xml\
	user_manual/zh-cn/quick_start.xml\
	user_manual/zh-cn/user_manual.xml
