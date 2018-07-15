// File: assemblerlistingpane.h
/*
    Pep9 is a virtual machine for writing machine language and assembly
    language programs.
    
    Copyright (C) 2009  J. Stanley Warford, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ASSEMBLERLISTINGPANE_H
#define ASSEMBLERLISTINGPANE_H

#include <QWidget>
#include "pepmicrohighlighter.h"
#include "enu.h"
#include "pepasmhighlighter.h"
namespace Ui {
    class AssemblerListingPane;
}
class AssemblerListingPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(AssemblerListingPane)
public:
    explicit AssemblerListingPane(QWidget *parent = 0);
    virtual ~AssemblerListingPane();
    void setAssemblerListing(QStringList assemblerListingList);
    void clearAssemblerListing();

    bool isModified();
    // Post: Returns true if the assembler listing pane has been modified

    QString toPlainText();
    // Post: Contents of the source code pane are returned

    void setCurrentFile(QString string);
    // Post: Title bar of the window is set to "Assembler Listing - 'string'"

    const QFile& getCurrentFile() const;
    // Post: Returns the file associated with the pane.

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus


    void copy();
    // Copies selected text to the clipboard

    
    void setFocus();
    // Post: the text edit has focus
    
    bool isEmpty();
    // Post: returns if the assembler listing is empty

public slots:
    void onFontChanged(QFont font);

private:
    Ui::AssemblerListingPane *ui;

    PepASMHighlighter *pepHighlighter;

    QFile currentFile;

    void mouseReleaseEvent(QMouseEvent *);

    void mouseDoubleClickEvent(QMouseEvent *);

signals:

    void labelDoubleClicked(Enu::EPane pane);
};

#endif // ASSEMBLERLISTINGPANE_H
