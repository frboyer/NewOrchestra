/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axGC.h"
#include "axWindow.h"
#include "axMath.h"

#include "axApp.h"
#include "axCore.h"

#include <cmath>

axGC::axGC(axWindow* win)//:
//_font("tt")
{
    _win = win;
}

axFloatRect axGC::RectToFloatRect(const axRect& rect)
{
	return axFloatRect(axFloat(rect.position.x), axFloat(rect.position.y),
					   axFloat(rect.size.x), axFloat(rect.size.y));
}

void axGC::SetColor(const axDouble& r, const axDouble& g, const axDouble& b)
{
	glColor4d(r, b, g, 1.0);
}

void axGC::SetColor(const axDouble& r,
                    const axDouble& g,
                    const axDouble& b,
                    const axDouble& a)
{
    glColor4d(r, g, b, a);
}

void axGC::SetColor(const axColor& color)
{
	glColor4d(color.GetRed(),
              color.GetGreen(),
              color.GetBlue(),
              color.GetAlpha());
}

void axGC::SetColor(const axColor& color, const float& alpha)
{
	glColor4d(color.GetRed(), color.GetGreen(), color.GetBlue(), alpha);
}

void axGC::DrawRectangle(const axRect& rect)
{    
    axFloatRect frect = RectToFloatRect(rect);
    axRectFloatPoints points = frect.GetPoints(); // Order : bl, tl, tr, br.

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_DOUBLE, 0, &points);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void axGC::DrawRoundedRectangle(const axRect& rect)
{
	(rect);
    /// @todo.
}

void axGC::DrawRectangleContour(const axRect& rect, float linewidth)
{
//    axMatrix4 mview_before(GL_MODELVIEW_MATRIX);
//    axMatrix4 mview;
//    mview.Identity().Translate(_win->GetAbsoluteRect().position).Process();
    
	axFloatRect frect = RectToFloatRect(rect);
//	frect.position.x  -= floor(_win->GetScrollDecay().x);
//	frect.position.y  -= _win->GetScrollDecay().y;
    
	// Note that OpenGL coordinate space has no notion of integers, 
	// everything is a float and the "centre" of an OpenGL pixel is 
	// really at the 0.5,0.5 instead of its top-left corner. 
	// Therefore, if you want a 1px wide line from 0,0 to 10,10 inclusive, 
	// you really had to draw a line from 0.5,0.5 to 10.5,10.5. 
	frect.position.x -= 0.5; // Seems to be only on linux and mac.

	glLineWidth((GLfloat)linewidth);
    
    axRectFloatPoints points = frect.GetPoints(); // Order : bl, tl, tr, br.
    GLubyte indices[] = {1,2, 2,3, 3,0, 0,1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_DOUBLE, 0, &points); // The value of z defaults is 0.
    glDrawElements(GL_LINE_LOOP, 8, GL_UNSIGNED_BYTE, indices);
    glDisableClientState(GL_VERTEX_ARRAY);

//    mview_before.Load();
    
    glLineWidth(1.0f);
}

void axGC::DrawTexture(GLuint texture, const axRect& rect, axColor color)
{
	(color);
//	axPoint pos = rect.position + _win->GetAbsoluteRect().position;
    axPoint pos = rect.position;
//	pos.x  -= _win->GetScrollDecay().x;
//	pos.y  -= _win->GetScrollDecay().y;

	//axColorStruct c = color.GetColorStruct();
	//glColor4f(c.r, c.g, c.b, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
//    glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture);

//	axREMOVE_ON_RELEASE
//	(
//		GLenum err = glGetError();
//
//		if (err != GL_NO_ERROR)
//		{
//			DSTREAM(3) << "GL TEXTURE ERROR : " << " " <<
//				gluErrorString(err) << endl;
//		}
//	)
//    glColor4d(1.0, 1.0, 1.0, 1.0);
	glDepthMask(GL_TRUE);
	axSize img_size = rect.size;

	glBegin(GL_QUADS);
	
	// Bottom left.
	glTexCoord2d(0.0, 0.0);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(0.0, 1.0);
	glVertex2d(pos.x, pos.y + img_size.y);

	// Top right.
	glTexCoord2d(1.0, 1.0);
	glVertex2d(pos.x + img_size.x, pos.y + img_size.y);

	// Buttom right.
	glTexCoord2d(1.0, 0.0);
	glVertex2d(pos.x + img_size.x, pos.y);

	glEnd();

//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

//void axGC::DrawWindowBuffer()
//{
//    glEnable(GL_TEXTURE_2D);
//    //glEnable(GL_BLEND);
//    
//    // Destionation funciton.
//    //glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
//		GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//    
//    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//    axFloatRect rect(RectToFloatRect(_win->GetShownRect()));
//    axFloatPoint pos(0.0, 0.0);
//    axFloatSize size = rect.size;
//
//    glBindTexture(GL_TEXTURE_2D, _win->GetWindowBufferTexture());
//
//	std::cout << "Draw back buffer texture quad : " << _win->GetId() << std::endl;
//	glColor4d(1.0, 1.0, 1.0, 1.0);
//    glBegin(GL_QUADS);
//    
//    // Bottom left.
//    glTexCoord2d(0.0, 0.0);
//    glVertex2d(pos.x, pos.y);
//    
//    // Top left.
//    glTexCoord2d(0.0, 1.0);
//    glVertex2d(pos.x, pos.y + size.y);
//    
//    // Top right.
//    glTexCoord2d(1.0, 1.0);
//    glVertex2d(pos.x + size.x, pos.y + size.y);
//    
//    // Buttom right.
//    glTexCoord2d(1.0, 0.0);
//    glVertex2d(pos.x + size.x, pos.y);
//    
//    glEnd();
//    
//    //	glDisable(GL_BLEND);
//    glDisable(GL_TEXTURE_2D);
//    
//
//    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}

//glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

struct axRectPointsOrder
{
    axRectPointsOrder(){}
    axRectPointsOrder(const axRectFloatPoints& points):
    top_left(points.top_left),
    top_right(points.top_right),
    bottom_left(points.bottom_left),
    bottom_right(points.bottom_right)
    {
    }
    axRectPointsOrder(const axFloatPoint& tl,
                      const axFloatPoint& tr,
                      const axFloatPoint& bl,
                      const axFloatPoint& br):
    top_left(tl), top_right(tr), bottom_left(bl),bottom_right(br)
    {
        
    }
    
    axFloatPoint top_left, top_right, bottom_left, bottom_right;
};
void axGC::DrawImage(axImage* img, const axPoint& position, double alpha)
{
//	axPoint pos = position + _win->GetAbsoluteRect().position;
    axPoint pos = position;
	//pos -= _win->GetScrollDecay();

	glColor4f(1.0f, 1.0f, 1.0f, float(alpha));

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, img->GetTexture());
	glDepthMask(GL_TRUE);
	axSize img_size = img->GetSize();

	// OpenGL stores texture upside down so glTexCoord2d must be flipped.
	glBegin(GL_QUADS);
	// Buttom left.
	glTexCoord2d(0.0, 1.0);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(0.0, 0.0);
	glVertex2d(pos.x, pos.y + img_size.y);

	// Top right.
	glTexCoord2d(1.0, 0.0);
	glVertex2d(pos.x + img_size.x, pos.y + img_size.y);

	// Buttom right.
	glTexCoord2d(1.0, 1.0);
	glVertex2d(pos.x + img_size.x, pos.y);
	glEnd();
//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void axGC::DrawImageResize(axImage* img, const axPoint& position, const axSize& size, double alpha)
{
//	axPoint pos = position + _win->GetAbsoluteRect().position;
    axPoint pos = position;
	//pos -= _win->GetScrollDecay();

	glColor4f(1.0f, 1.0f, 1.0f, float(alpha));

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, img->GetTexture());
	glDepthMask(GL_TRUE);
	axSize img_size = size;

	// OpenGL stores texture upside down so glTexCoord2d must be flipped.
	glBegin(GL_QUADS);
	// Buttom left.
	glTexCoord2d(0.0, 1.0);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(0.0, 0.0);
	glVertex2d(pos.x, pos.y + img_size.y);

	// Top right.
	glTexCoord2d(1.0, 0.0);
	glVertex2d(pos.x + img_size.x, pos.y + img_size.y);

	// Buttom right.
	glTexCoord2d(1.0, 1.0);
	glVertex2d(pos.x + img_size.x, pos.y);
	glEnd();

//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void axGC::DrawPartOfImage(axImage* img,
                           const axPoint& posInImage,
                           const axSize& sizeInImage,
                           const axPoint& position,
                           double alpha)
{
    //	axPoint pos = position + _win->GetAbsoluteRect().position;
    
    axPoint pos = position;
    //pos -= _win->GetScrollDecay();
    
    axSize img_size = img->GetSize();
    
    double img_x = (posInImage.x + sizeInImage.x) / double(img_size.x),
    img_y = 1.0 - (posInImage.y + sizeInImage.y) / double(img_size.y);
    
    double x = posInImage.x / double(img_size.x);
    double y = 1.0 - posInImage.y / double(img_size.y);
    
    glColor4f(1.0f, 1.0f, 1.0f, float(alpha));
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, img->GetTexture());
    glDepthMask(GL_TRUE);
    
    // OpenGL stores texture upside down so glTexCoord2d must be flipped.
    glBegin(GL_QUADS);
    
    // Buttom left.
    glTexCoord2d(x, y);
    glVertex2d(pos.x, pos.y);
    
    // Top left.
    glTexCoord2d(x, img_y);
    glVertex2d(pos.x, pos.y + sizeInImage.y-1);
    
    // Top right.
    glTexCoord2d(img_x, img_y);
    glVertex2d(pos.x + sizeInImage.x-1, pos.y + sizeInImage.y-1);
    
    // Buttom right.
    glTexCoord2d(img_x, y);
    glVertex2d(pos.x + sizeInImage.x-1, pos.y);
    glEnd();
    //	glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}


void axGC::DrawPartOfImageResize(axImage* img,
					 const axPoint& posInImage,
					 const axSize& sizeInImage,
					 const axRect& rect,
                     double alpha)
{
    axPoint pos = rect.position;
	axSize img_size = img->GetSize();

	double img_x = (posInImage.x + sizeInImage.x) / double(img_size.x),
		img_y = 1.0 - (posInImage.y + sizeInImage.y) / double(img_size.y);

	double x = posInImage.x / double(img_size.x);
	double y = 1.0 - posInImage.y / double(img_size.y);

	glColor4f(1.0f, 1.0f, 1.0f, float(alpha));

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, img->GetTexture());
	glDepthMask(GL_TRUE);
	
	// OpenGL stores texture upside down so glTexCoord2d must be flipped.
	glBegin(GL_QUADS);
	
	// Buttom left.
	glTexCoord2d(x, y);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(x, img_y);
	glVertex2d(pos.x, pos.y + rect.size.y);

	// Top right.
	glTexCoord2d(img_x, img_y);
	glVertex2d(pos.x + rect.size.x, pos.y + rect.size.y);

	// Buttom right.
	glTexCoord2d(img_x, y);
	glVertex2d(pos.x + rect.size.x, pos.y);
	glEnd();
//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void axGC::DrawString(axFont& font,
                      const std::string& text,
                      const axPoint& pos)
{
    if(font)
    {
        int x = pos.x;
        
        for (unsigned int i = 0; i < text.size(); i++)
        {
            font.SetChar(text[i]);
            axPoint delta = font.GetDelta();
            
            DrawTexture(font.GetTexture(),
                        axRect(axPoint(x + delta.x, pos.y - delta.y + font.GetFontSize()),
                               font.GetSize()));
            
            x += font.GetNextPosition();
        }
    }

}

axPoint axGC::DrawChar(axFont& font,
                       const char& key,
                       const axPoint& pos)
{
    if(font)
    {
        int x = pos.x;
        
        font.SetChar(key);
        axPoint delta = font.GetDelta();
        
        DrawTexture(font.GetTexture(),
                    axRect(axPoint(x + delta.x, pos.y - delta.y + font.GetFontSize()),
                           font.GetSize()));
        
        x += font.GetNextPosition();
        
        return axPoint(x, pos.y);
    }

    return axPoint(0, 0);
}


// Just blocking x axis for now.
void axGC::BlockDrawing(const axRect& rect)
{
    axMatrix4 before_proj(GL_MODELVIEW);
    axMatrix4 proj;
    proj.Identity().Load();
    
//    axRect r = rect;
    
    axPoint abso(_win->GetAbsoluteRect().position);
    
    glScissor(abso.x + rect.position.x,
              axApp::GetInstance()->GetCore()->GetGlobalSize().y -
              (abso.y + rect.position.y + rect.size.y),
              rect.size.x,
              rect.size.y);
    
    glEnable(GL_SCISSOR_TEST);
    before_proj.Load();
}

void axGC::UnBlockDrawing()
{
    glDisable(GL_SCISSOR_TEST);
}

void axGC::DrawStringAlignedCenter(axFont& font,
                                   const string& text,
								   //const axPoint& pos,
								   const axRect& rect)
{
    if(font)
    {
        int length = 0;
        int height = 0;
        for (unsigned int i = 0; i < text.size(); i++)
        {
            font.SetChar(text[i]);
            length += font.GetNextPosition();
            
            if (font.GetSize().y > height)
                height = font.GetSize().y;
        }
        
        axPoint pos(int(rect.position.x + (rect.size.x - length) * 0.5),
                    int(rect.position.y + ceil((rect.size.y - height) * 0.5)));
        
        int x = pos.x;
        for (unsigned int i = 0; i < text.size(); i++)
        {
            font.SetChar(text[i]);
            axPoint delta = font.GetDelta();
            
            DrawTexture(font.GetTexture(),
                        axRect(axPoint(x + delta.x, pos.y - delta.y + height),
                               font.GetSize()));
            
            x += font.GetNextPosition();
        }
    }
}

void axGC::DrawRectangleColorFade(const axRect& rectangle,
								  const axColor& c1, const float& alpha1,
								  const axColor& c2, const float& alpha2)
{
//	axFloatRect rect = RectToFloatRect(rectangle + _win->GetAbsoluteRect().position);
    axFloatRect rect = RectToFloatRect(rectangle);
	//rect.position.x  -= _win->GetScrollDecay().x;
	//rect.position.y  -= _win->GetScrollDecay().y;

	glBegin(GL_QUADS);
	SetColor(c1, alpha1);
	glVertex3f(float(rect.position.x), float(rect.position.y), 0.0f); // Bottom left.

	//SetColor(c1);
	glVertex3f(float(rect.position.x + rect.size.x),
			   float(rect.position.y), 0.0f); // Bottom Right.

	SetColor(c2, alpha2);
	glVertex3f(float(rect.position.x + rect.size.x),
			   float(rect.position.y + rect.size.y), 0.0f); // Top Right.

	//SetColor(c2);
	glVertex3f(float(rect.position.x),
			   float(rect.position.y + rect.size.y), 
			   0.0f); // Top Left
	glEnd();
}

void axGC::DrawRectangleColorFade(const axRect& rectangle,
                                  const axColor& c1,
                                  const axColor& c2)
{
//    axFloatRect rect = RectToFloatRect(rectangle + _win->GetAbsoluteRect().position);
    axFloatRect rect = RectToFloatRect(rectangle);
    
    glBegin(GL_QUADS);
    SetColor(c1);
    glVertex3f(float(rect.position.x), 
			   float(rect.position.y), 0.0f); // Bottom left.
    
    glVertex3f(float(rect.position.x + rect.size.x),
               float(rect.position.y), 0.0f); // Bottom Right.
    
    SetColor(c2);
	glVertex3f(float(rect.position.x + rect.size.x),
			   float(rect.position.y + rect.size.y), 0.0f); // Top Right.
    
	glVertex3f(float(rect.position.x),
			   float(rect.position.y + rect.size.y), 0.0f); // Top Left
    glEnd();
}

void axGC::DrawLines(const vector<axPoint>& pts, float width)
{
//	axPoint real_pos = _win->GetAbsoluteRect().position;
//    axPoint real_pos = _win->GetRect().position;
//	real_pos.x -= _win->GetScrollDecay().x;
//	real_pos.y -= _win->GetScrollDecay().y;
//
//    glPushMatrix();
//    glTranslated(real_pos.x, real_pos.y, 0.0);

	glLineWidth(width);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, pts.data());
    glDrawArrays(GL_LINES, 0, int(pts.size()));
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}


void axGC::DrawSmouthLine(const axPoint& pt1, const axPoint& pt2)
{
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glBegin(GL_LINES);
    glVertex2f(float(pt1.x), float(pt1.y));
    glVertex2f(float(pt2.x), float(pt2.y));
    glEnd();
    
    glDisable(GL_LINE_SMOOTH);
}

void axGC::DrawLine(const axPoint& pt1, const axPoint& pt2, float width)
{

    
//	axPoint real_pos = _win->GetAbsoluteRect().position;
//    axPoint real_pos = _win->GetRect().position;
//	real_pos.x  -= _win->GetScrollDecay().x;
//	real_pos.y  -= _win->GetScrollDecay().y;

    axPoint p1 = pt1;// - _win->GetScrollDecay().x;;// + real_pos;
    axPoint p2 = pt2;// - _win->GetScrollDecay().y;// + real_pos;

//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex2f(float(p1.x), float(p1.y));
	glVertex2f(float(p2.x), float(p2.y));
	glEnd();
    
//    glDisable(GL_LINE_SMOOTH);
}

void axGC::DrawPoint(const axPoint& pt, const int& size)
{
    glEnable(GL_POINT_SMOOTH);
    glPointSize(float(size));
    glBegin(GL_POINTS);
	glVertex2f(float(pt.x), float(pt.y));
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

void axGC::DrawLineCubic(const axPoint& pt1, const axPoint& pt2)
{
    // H1(t) = 2t^3 - 3t^2 + 1
    // H2(t) = -2t^3 + 3t^2
    // H3(t) = t^3 - 2t^2 + t
    // H4(t) = t^3 - t^2
    // V1 = Tengente at point 1.
    // V2 = Tengente at point 2.
    // P(t) = [H1(t) H2(t) H3(t) H4(t)] * [P1 P2 V1 V2];
    // P(t) = H1(t) * P1.x + H2(t) * P2.x + H3(t) *
    
//    double t = 0.0;
    
    double v1x = 100.0, v1y = 200;
    double v2x = 100.0, v2y = -200;
    
    double pp_x = pt1.x, pp_y = pt1.y;
    
    glBegin(GL_LINES);
    for(int i = 1; i < 101; i++)
    {
        double t = i / double(101.0);
        
        double h1 = 2.0 * pow(t, 3.0) - 3.0 * pow(t, 2.0) + 1.0;
        double h2 = -2.0 * pow(t, 3.0) + 3.0 * pow(t, 2.0);
        double h3 = pow(t, 3.0) - 2.0 * pow(t, 2.0) + t;
        double h4 = pow(t, 3.0) - pow(t, 2.0);
        
        double p_x = h1 * pt1.x + h2 * pt2.x + h3 * v1x + h4 * v2x;
        double p_y = h1 * pt1.y + h2 * pt2.y + h3 * v1y + h4 * v2y;
        
		glVertex2f(float(pp_x), float(pp_y));
		glVertex2f(float(p_x), float(p_y));
        
        pp_x = p_x;
        pp_y = p_y;
    }
    glEnd();
    
    
//    glVertex2f(pt1.x, pt1.y);
//    glVertex2f(pt2.x, pt2.y);
    
}

void axGC::SetLineWidth(const double& width)
{
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glLineWidth(float(width));

}

void axGC::SeDefaultLine()
{
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glLineWidth(1.0);
}

void axGC::DrawCircle(const axPoint& pos,
                      const double& radius,
                      const int& nSegments)
{ 
//	axPoint real_pos = _win->GetAbsoluteRect().position;
    axPoint real_pos = pos;//_win->GetRect().position;

	//real_pos -= _win->GetScrollDecay();

//	real_pos += pos;

	glBegin(GL_LINE_LOOP); 
	for(int i = 0; i < nSegments; i++)
	{
        // Get the current angle.
		double theta = 2.0f * M_PI * double(i) / double(nSegments);

		double x = radius * cos(theta);
		double y = radius * sin(theta);

		glVertex2d(x + real_pos.x, y + real_pos.y);
	} 
	glEnd(); 
}