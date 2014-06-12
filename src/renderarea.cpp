 /****************************************************************************
 **
 ** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include <QtGui>
 #include <QToolTip>
 #include <random>
 #include <iostream>
 #include "renderarea.h"
 #include "ComparisonMatrix.h"
 RenderArea::RenderArea(ComparisonMatrix &mat,QWidget *parent)
     :QWidget(parent), matrix_(mat) 
 {
     antialiased = false;
     click_value = "";
     setAutoFillBackground(true);
     keys = matrix_.get_keys();
 }

QString RenderArea::getText() const{
    return  click_value;
}

void RenderArea::mousePressEvent(QMouseEvent *event)
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
     emit MyClicked();
 }

 QSize RenderArea::minimumSizeHint() const
 {
     return QSize(100, 100);
 }

 QSize RenderArea::sizeHint() const
 {
     return QSize(400, 400);
 }


 void RenderArea::setAntialiased(bool antialiased)
 {
     this->antialiased = antialiased;
     update();
 }


 void RenderArea::paintEvent(QPaintEvent * /* event */)
 {
    //Similarity operator()(const std::string&, const std::string& );
   
    //detect min and max 
 
     QPainter painter(this);
     if (antialiased)
         painter.setRenderHint(QPainter::Antialiasing, true);


     painter.setRenderHint(QPainter::Antialiasing, false);
     painter.setPen(QColor(255,255,255,255));
     painter.setBrush(Qt::NoBrush);
     painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

     
     double w = 1.*width()/keys.size();
     double h = 1.*(height())/keys.size();
     //std::random_device rd;
     //std::mt19937_64 gen(rd());
     //std::uniform_int_distribution<int> dis{0, 255};
     
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
