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
 #include <QToolTip>
 #include <random>
 #include <iostream>
 #include "ComparisionView.h"
 #include "ComparisonMatrix.h"
 
 ComparisionView::ComparisionView(ComparisonMatrix &mat,QWidget *parent)
     :QWidget(parent), matrix_(mat) 
 {
     antialiased = false;
     click_value = "";
     setAutoFillBackground(true);
     keys = matrix_.get_keys();
 }

QString ComparisionView::getText() const{
    return  click_value;
}

void ComparisionView::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton) {
         int x = event->x();
         int y = event->y();
         int i = keys.size()*x/width();
         int j = keys.size()*y/height();
         //std::cout<<"Clicked at "<<i<<":"<<j  <<std::endl;
         std::string s = keys[i]+ " vs. "+keys[j]+": ";
         s += std::to_string( matrix_(keys[i],keys[j]).val );
         click_value = QString::fromStdString(s);
         QToolTip::showText( this->mapToGlobal( QPoint( x, y ) ), click_value, 
                             this, QRect(0,0,width(),height()) );
         
     }
     
     else if (event->button() == Qt::RightButton) {
         int x = event->x();
         int y = event->y();
         int i = keys.size()*x/width();
         int j = keys.size()*y/height();
         matrix_.visual_diff(keys[i],keys[j]);
     }
     emit view_clicked();
 }

 QSize ComparisionView::minimumSizeHint() const
 {
     return QSize(100, 100);
 }

 QSize ComparisionView::sizeHint() const
 {
     return QSize(400, 400);
 }


 void ComparisionView::setAntialiased(bool antialiased)
 {
     this->antialiased = antialiased;
     update();
 }


 void ComparisionView::paintEvent(QPaintEvent * /* event */)
 {
 
     QPainter painter(this);
     if (antialiased)
         painter.setRenderHint(QPainter::Antialiasing, true);


     painter.setRenderHint(QPainter::Antialiasing, false);
     painter.setPen(QColor(255,255,255,255));
     painter.setBrush(Qt::NoBrush);
     painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

     
     double w = 1.*width()/keys.size();
     double h = 1.*(height())/keys.size();
     
     double min = matrix_.get_min();
     double max = matrix_.get_max();
        
     for(uint i=0; i<keys.size();++i){
        for(uint j= 0; j<keys.size();++j){
            //int v = dis(gen);
            int v = 0;
            if(i!=j and min!=max)
                v =  255*(matrix_(keys[i],keys[j]).val-min)/(max-min);
            painter.fillRect(QRect(i*w, j*h, w+1, h+1),QColor(255,255-v,255-v,255));
        }    
    }       
 }
