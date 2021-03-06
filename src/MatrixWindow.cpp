/**
Copyright (c) 2014 Torsten J. Gellert

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**/


 #include <QtGui>
 #include <QLabel>
 #include "ComparisionView.h"
 #include "MatrixWindow.h"

 const int IdRole = Qt::UserRole;

 MatrixWindow::MatrixWindow(ComparisonMatrix &matrix)
 {
     view = new ComparisionView(matrix);

     QGridLayout *mainLayout = new QGridLayout;
     bottomLabel     = new QLabel("");
     
     mainLayout->setRowStretch(0, 1);
     mainLayout->setRowStretch(1, 0);
     mainLayout->addWidget(view, 0, 0, 1, 4);
     mainLayout->addWidget(bottomLabel, 1, 0, 1, 4);
     setLayout(mainLayout);
     setWindowTitle(tr("Similarity Matrix"));
     
     connect(view, SIGNAL(view_clicked()), this, SLOT(slotSetBottomText()));
 }


void MatrixWindow::slotSetBottomText(){
    bottomLabel->setText(view->getText());
}
