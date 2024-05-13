#include <QtWidgets>
#include <QCoreApplication>
#include <QDir>
#include <string>
#include <QSyntaxHighlighter>
#include <QColor>
#include <QInputDialog>
#include <QKeyEvent>
#include <QTextCursor>
#include <windows.h>

#define MAXLEN 100

const char* toUtf8(const char* ansi) {

}

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat identifierFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat commentFormat;
};

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QColor keywordColor("#006BBB");
    QColor numberColor("#FF0000");
    QColor identifierColor("#BB6FFF");
    QColor stringColor("#FF1FD5");
    QColor commentColor("#800080");

    keywordFormat.setForeground(keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);
    numberFormat.setForeground(numberColor);
    identifierFormat.setForeground(identifierColor);
    stringFormat.setForeground(stringColor);
    commentFormat.setForeground(commentColor);

    QStringList keywordPatterns;
    keywordPatterns << "\\bdef\\b" << "(?<=[^가-힣])함수(?=[^가-힣])"
                    << "\\bif\\b" << "(?<=[^가-힣])조건(?=[^가-힣])"
                    << "\\bfor\\b" << "(?<=[^가-힣])반복(?=[^가-힣])"
                    << "\\bgosub\\b" << "(?<=[^가-힣])순회(?=[^가-힣])"
                    << "\\bon\\b" << "(?<=[^가-힣])검사(?=[^가-힣])"
                    << "\\bread\\b" << "(?<=[^가-힣])읽기(?=[^가-힣])"
                    << "\\binput\\b" << "(?<=[^가-힣])입력(?=[^가-힣])"
                    << "\\bprint\\b" << "(?<=[^가-힣])출력(?=[^가-힣])"
                    << "\\bdata\\b" << "(?<=[^가-힣])자료(?=[^가-힣])"
                    << "\\bend\\b" << "(?<=[^가-힣])끝(?=[^가-힣])"
                    << "\\bnext\\b" << "(?<=[^가-힣])다음(?=[^가-힣])"
                    << "\\bor\\b"
                    << "\\band\\b"
                    << "\\bstop\\b" << "(?<=[^가-힣])중지(?=[^가-힣])"
                    << "\\breturn\\b" << "(?<=[^가-힣])복귀(?=[^가-힣])"
                    << "\\brestore\\b" << "(?<=[^가-힣])첫자료(?=[^가-힣])"
                    << "\\brem\\b" << "(?<=[^가-힣])주석(?=[^가-힣])"
                    << "\\bstep\\b" << "(?<=[^가-힣])폭(?=[^가-힣])"
                    << "\\bto\\b"
                    << "\\bnew\\b" << "(?<=[^가-힣])초기화(?=[^가-힣])"
                    << "\\bthen\\b" << "(?<=[^가-힣])참이면(?=[^가-힣])"
                    << "\\bdim\\b" << "(?<=[^가-힣])배열(?=[^가-힣])"
                    << "\\blet\\b" << "(?<=[^가-힣])변수(?=[^가-힣])"
                    << "\\bgoto\\b" << "(?<=[^가-힣])분기(?=[^가-힣])"
                    << "\\bcont\\b" << "(?<=[^가-힣])재개(?=[^가-힣])";


    QString numberPattern = "\\b\\d+\\b";
    // QString identifierPattern = "\\b\\$?[a-zA-Z_][a-zA-Z_0-9]*\\b";
    QString identifierPattern = "(?<=[^가-힣])[가-힣_][가-힣_0-9]*(?=[^가-힣])";
    QString stringPattern = "\"[^\"]*\"";
    QString commentPattern = "rem [^\n]*";

    rule.pattern = QRegularExpression(identifierPattern);
    rule.format = identifierFormat;
    highlightingRules.append(rule);

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    rule.pattern = QRegularExpression(numberPattern);
    rule.format = numberFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(stringPattern);
    rule.format = stringFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(commentPattern);
    rule.format = commentFormat;
    highlightingRules.append(rule);

}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

class MyWidget : public QWidget {
    Q_OBJECT
public:
    MyWidget(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        inputTextEdit = new QTextEdit(this);
        inputTextEdit->setPlaceholderText("your code here");

        SyntaxHighlighter *highlighter = new SyntaxHighlighter(inputTextEdit->document());

        layout->addWidget(inputTextEdit);

        outputTextEdit = new QTextEdit(this);
        outputTextEdit->setReadOnly(true);
        outputTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(outputTextEdit);

        QPushButton* button = new QPushButton("실행", this);
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, this, &MyWidget::run);

        setLayout(layout);
        createMenu();
        connect(inputTextEdit, &QTextEdit::textChanged, this, &MyWidget::markAsUnsaved);
    }

public slots:

    void run() {
        QString inputText = inputTextEdit->toPlainText();

        inputText.replace('\t', "    ");

        QProcess process;

        QString filePath = "C:\\Users\\iseogwon\\Desktop\\basic0818.exe";
        process.start(filePath);

        if (process.waitForStarted()) {
            // process.write(inputText.toUtf8());
            QStringList lines = inputText.split("\n");

            for (QString& line : lines) {
                if (line.contains("input")) {
                    int commaCount = line.count(",");
                    bool ok;
                    QString text;
                    QString info;
                    bool isString = false;
                    if (commaCount) {
                        info = "Input " + QString::number(commaCount + 1) + " pieces of data.";
                    }
                    else if (line.contains("$")) {
                        info = "Input string";

                        isString = true;
                    }
                    else {
                        info = "Input integer";
                    }
                    text = QInputDialog::getText(nullptr, "Input Dialog", info, QLineEdit::Normal, "", &ok);
                    /*
                     * 입력창 전환
                     * outputTextEdit->setFocus();
                    outputTextEdit->setReadOnly(false);
                    QString userInput;
                    while (true) {
                        QEventLoop loop;
                        connect(outputTextEdit, &QTextEdit::textChanged, &loop, &QEventLoop::quit);
                        loop.exec();

                        QString newText = outputTextEdit->toPlainText();
                        if (newText.contains('\n')) {
                            int endIndex = newText.indexOf('\n');
                            userInput = newText.left(endIndex);
                            break;
                        }
                    }*/
                    line.append(" ");

                    if (isString) {
                        line.append("\"" + text + "\"");
                    }
                    else {
                        line.append(text);
                    }

                }
                process.write(line.toUtf8());
                process.write("\n");
                process.waitForBytesWritten();
            }

            process.closeWriteChannel();

            if (process.waitForFinished(-1)) {
                QString outputText = process.readAllStandardOutput();
                outputTextEdit->setPlainText(outputText.mid(outputText.indexOf("\n") + 1));
            } else {
                outputTextEdit->setPlainText("failed");
            }
        }


        else {
            outputTextEdit->setPlainText("failed");
        }
        unsavedChanges = true;
    }

    void markAsUnsaved() {
        unsavedChanges = true;
    }

private:
    QTextEdit* inputTextEdit;
    QTextEdit* outputTextEdit;
    QString currentFilePath;
    bool unsavedChanges = false;

    void createMenu() {
        QMenuBar* menuBar = new QMenuBar(this);

        QMenu* fileMenu = new QMenu("파일", this);
        QAction* openAction = new QAction("열기", this);
        fileMenu->addAction(openAction);
        connect(openAction, &QAction::triggered, this, &MyWidget::openFile);
        QAction* saveAction = new QAction("저장", this);
        fileMenu->addAction(saveAction);
        connect(saveAction, &QAction::triggered, this, &MyWidget::saveFile);
        QAction* saveAsAction = new QAction("다른 이름으로 저장", this);
        fileMenu->addAction(saveAsAction);
        connect(saveAsAction, &QAction::triggered, this, &MyWidget::saveFileAs);
        QAction* exitAction = new QAction("종료", this);
        fileMenu->addAction(exitAction);
        connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

        QMenu* executeMenu = new QMenu("실행", this);
        QAction* executeAction = new QAction("실행", this);
        executeMenu->addAction(executeAction);
        connect(executeAction, &QAction::triggered, this, &MyWidget::executeFile);

        QMenu* helpMenu = new QMenu("도움말", this);
        QAction* aboutAction = new QAction("코베이직에 대하여", this);
        helpMenu->addAction(aboutAction);
        connect(aboutAction, &QAction::triggered, this, &MyWidget::showAboutDialog);

        menuBar->addMenu(fileMenu);
        menuBar->addMenu(executeMenu);
        menuBar->addMenu(helpMenu);

        layout()->setMenuBar(menuBar);
    }
    void openFile() {
        QString filePath = QFileDialog::getOpenFileName(this, "파일 열기", QDir::homePath(), "Text files (*.txt);;All files (*)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString content = in.readAll();
                inputTextEdit->setPlainText(content);
                file.close();
                currentFilePath = filePath;
            } else {
                QMessageBox::warning(this, "Error", "파일을 열 수 없습니다.");
            }
        }
    }

    void saveFile() {
        if (currentFilePath.isEmpty()) {
            saveFileAs();
        } else {
            QFile file(currentFilePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << inputTextEdit->toPlainText();
                file.close();
            } else {
                QMessageBox::warning(this, "Error", "파일을 저장할 수 없습니다.");
            }
        }
    }

    void saveFileAs() {
        QString filePath = QFileDialog::getSaveFileName(this, "다른 이름으로 저장", QDir::homePath(), "Text files (*.txt);;All files (*)");
        if (!filePath.isEmpty()) {
            currentFilePath = filePath;
            saveFile();
        }
    }

    void executeFile() {
        run();
    }

    void showAboutDialog() {
        QString aboutText = "코베이직에 대하여\n\n"
                                "버전: 1.0\n";

        QMessageBox::about(this, "About 코베", aboutText);
    }



protected:

void closeEvent(QCloseEvent* event) override {
        if (unsavedChanges) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("변경사항 저장");
            msgBox.setText("변경사항을 저장하시겠습니까?");

            QPushButton* saveButton = msgBox.addButton("저장", QMessageBox::AcceptRole);
            msgBox.addButton("저장 안 함", QMessageBox::RejectRole);
            QPushButton* cancelButton = msgBox.addButton("취소", QMessageBox::RejectRole);

            msgBox.setDefaultButton(saveButton);
            msgBox.exec();

            if (msgBox.clickedButton() == saveButton) {
                saveFile();
            } else if (msgBox.clickedButton() == cancelButton) {
                event->ignore();
                return;
            }
        }
        event->accept();
    }
};


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MyWidget widget;
    widget.setMinimumSize(800, 600);
    widget.show();
    return app.exec();
}

#include "main.moc"
