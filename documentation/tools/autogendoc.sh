#!/bin/sh

DOCBOOK_ROOT=../docbook
DOCBOOK_XSL_ROOT=$DOCBOOK_ROOT/docbook-xsl-ns-1.76.1
DOC_SRC=../src
FOP_ROOT=~/work/fop-1.0
FOP_CONF=../fop/conf

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DOCBOOK_XSL_ROOT

echo '===== Unzip docbook package ====='
rm -rf $DOCBOOK_ROOT/docbook-xsl-ns-1.76.1
unzip -d $DOCBOOK_ROOT/ $DOCBOOK_ROOT/docbook-xsl-ns-1.76.1.zip

echo '===== Generate html document ====='

xsltproc -o $DOC_SRC/user_manual/zh-cn/user_manual.html  \
    --stringparam section.autolabel 1                    \
    --stringparam html.stylesheet                        \
    $DOCBOOK_ROOT/css/stylesheet.css                     \
    $DOCBOOK_ROOT/xsl/mydocbook.xsl                      \
    $DOC_SRC/user_manual/zh-cn/user_manual.xml     || exit 1


echo '===== Generate fo document ====='

xsltproc --output $DOC_SRC/user_manual/zh-cn/user_manual.fo   \
    --stringparam use.extensions 0                 \
    --stringparam fop1.extensions 1                 \
    --stringparam l10n.gentext.language zh_cn       \
    --stringparam hyphenate false                   \
    --stringparam body.font.family      FZFangSong      \
    --stringparam monospace.font.family FZFangSong      \
    --stringparam dingbat.font.family   FZFangSong      \
    --stringparam title.font.family     FZFangSong      \
    --stringparam paper.type            A4         \
    $DOCBOOK_XSL_ROOT/fo/docbook.xsl                \
    $DOC_SRC/user_manual/zh-cn/user_manual.xml      || exit 1

tidy -quiet -utf8 -indent -modify -xml $DOC_SRC/user_manual/zh-cn/user_manual.fo

echo '===== Generate pdf document ====='
$FOP_ROOT/fop -c $FOP_CONF/userconfig.xml       \
    $DOC_SRC/user_manual/zh-cn/user_manual.fo   \
    $DOC_SRC/user_manual/zh-cn/user_manual.pdf

echo 'Document generated.'