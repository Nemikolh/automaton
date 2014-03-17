#ifndef LUAHIGHLIGHTER_H
#define LUAHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegExp>
#include <QVector>

namespace nkh {

class LuaHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit LuaHighlighter(QTextDocument *parent = 0);

private:
    void highlightBlock(const QString &text) override;

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

} /* namespace nkh */

#endif // LUAHIGHLIGHTER_H
