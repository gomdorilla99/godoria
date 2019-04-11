#include "StdAfx.h"
#include "StoryBoard.h"


#include "resource.h"       // 주 기호입니다.
#include "math.h"

#define ZOOMMAX 1.5

CStoryBoard::CStoryBoard(void)
{
	int i;
	nItem = 4;
	mMousePoint.x=0;
	mMousePoint.y=0;
	for(i=0;i<nItem;i++){
		Velocity[i].x=0;
		Velocity[i].y=0;
	}

	nGo=0;
	nNG=0;
	nPoint=0;
	mLock = 1;
	stage = 0;

	//BackBrush.CreateSolidBrush(RGB(75,75,75));
	//BackBrush.CreateSolidBrush(RGB(100,75,75));
	BackBrush.CreateSolidBrush(RGB(75,100,75));
	BackShadowBrush.CreateSolidBrush(RGB(15,55,18));
	
	NULLPen.CreateStockObject(NULL_PEN);
	WhitePen[0].CreatePen(PS_SOLID,4,RGB(255,220,220));
	WhitePen[1].CreatePen(PS_SOLID,3,RGB(255,210,210));
	WhitePen[2].CreatePen(PS_SOLID,2,RGB(255,200,200));
	WhitePen[3].CreatePen(PS_SOLID,1,RGB(255,190,190));
	WhitePen[4].CreatePen(PS_SOLID,2,RGB(255,180,180));
	
	curBitmap.LoadBitmap(IDB_CARDFACE); // 비트맵 객체에 리소스 연결
	CardBack.LoadBitmap(IDB_CARDBACK);
	RedCard.CreatePatternBrush(&CardBack);
}

CStoryBoard::~CStoryBoard(void)
{
	curBitmap.DeleteObject();
}


CPoint CStoryBoard::Gravity(CPoint p1, CPoint p2,int m1, int m2)
{
	CPoint temp;
	double rsq = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)+40;
	double r=sqrt((double)rsq);
	if(r>0){
		temp.x= GForce*(m1*m2)*(p1.x -p2.x)/r/rsq; 
		temp.y= GForce*(m1*m2)*(p1.y -p2.y)/r/rsq;
	}else
	{
		temp.x=temp.y=0;
	}
	return temp;
}

int CStoryBoard::GravityX(CPoint p1, int X,int m1, int m2)
{
	int temp;
	double rsq = (p1.x - X)*(p1.x - X)+40;
	double r=sqrt((double)rsq);
	temp =GForce*(m1*m2)*(p1.x - X )/rsq; 
	if(p1.x>mRegion.left && p1.x<mRegion.right)
	{
		temp;
	}
	else
	{
		temp = 0;//-temp;///
	}
	return temp;  
}

int CStoryBoard::GravityY(CPoint p1, int Y,int m1, int m2)
{
	int temp;
	double rsq = (p1.y - Y)*(p1.y - Y)+40;
	double r=sqrt((double)rsq);
	temp = GForce*(m1*m2)*( p1.y-Y)/rsq; 
	if(p1.y>mRegion.top && p1.y<mRegion.bottom)
	{
		temp;//0;
	}
	else
	{
		temp = 0;//-temp;//
	}
	return temp; 
}


int CStoryBoard::DrawCard(CDC *pDC,int index)
{	
	//CPoint refPoint;
	CPoint reactPoint;
	CBitmap *oldBitmap;
	CBrush *pOldBrush;
	CPen *pOldPen;
	unsigned int distance;
	double factor;

	unsigned int step = mRegion.Width() / (nItem+1);
	if (step > 60) step= 60;
	CRect temp;
	oldBitmap=MemoryDC.SelectObject(&curBitmap); // 메모리 DC가 curBitmap을 선택
	mrefPoint[index].x = mRegion.right-step*(nItem+1)+(step*(index*2+1))/2;
	mrefPoint[index].y = (mRegion.top+2*mRegion.bottom)/3;
	factor = 1.;
	distance = (mrefPoint[index].x - mMousePoint.x)*(mrefPoint[index].x - mMousePoint.x) +
				   (mrefPoint[index].y - mMousePoint.y)*(mrefPoint[index].y - mMousePoint.y);

	for(int s=0;s<2;s++)
	{

			if(sqrt(distance+20.) < 100)
			{
				if(distance <2000){
					factor = ZOOMMAX;
								}
				else
				{
					factor = 100./sqrt(distance+20.);
				}

				if(factor>ZOOMMAX) factor = ZOOMMAX;
				if(mrefPoint[index].x > mMousePoint.x )
					reactPoint.x = mrefPoint[index].x + sqrt((double)distance)/1000.;
				else
					reactPoint.x = mrefPoint[index].x - sqrt((double)distance)/1000.;
				reactPoint.y = mrefPoint[index].y - 2000./(sqrt((double)distance)+30);
			}else
			{
				factor = 1.;
				reactPoint.x = mrefPoint[index].x;
				reactPoint.y = mrefPoint[index].y - 2000./sqrt((double)distance+30);
			}
			if(s==1){
				temp.left = reactPoint.x-(step/2)*factor;
				temp.top = reactPoint.y-(step/2)*140/88*factor;
				temp.right = reactPoint.x+(step/2)*factor;
				temp.bottom = reactPoint.y+(step/2)*140/88*factor;
				//pDC->RoundRect(temp.left-2,temp.top-2,temp.right+2,temp.bottom+2,8,8);	

				BLENDFUNCTION bf;      // structure for alpha blending
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = AC_SRC_OVER;  // use source alpha 
				bf.SourceConstantAlpha = 100 * factor+100;  // opaque (disable constant alpha)

			   //AlphaBlend(hdcwnd, x, y, 30, 30, hMemDC, 0, 0, 30, 30, bf);
				//pDC->AlphaBlend(temp.left+1,temp.top+1,temp.Width()-2,temp.Height()-2,&MemoryDC,
				//	((stage*4+index)%8)*73,((stage*4+index)/8)*120,73,120,bf);  	
				
				pDC->AlphaBlend(temp.left + 1, temp.top + 1, temp.Width() - 2, temp.Height() - 2, &MemoryDC,
					((stage * 4 + index) % 8) * 97, ((stage * 4 + index)/8) * 145, 88, 140, bf);

				//pDC->StretchBlt(temp.left+1,temp.top+1,temp.Width()-2,temp.Height()-2,&MemoryDC,
				//	(index%6)*73,(index/6)*120,73,120,SRCCOPY); 				
			}else
			{
				pOldPen = pDC->SelectObject(&NULLPen);
				pOldBrush  = pDC->SelectObject(&BackShadowBrush);
				reactPoint.y = mrefPoint[index].y + 2000./sqrt((double)distance+30);
				temp.left = reactPoint.x-(step/2)*factor;
				temp.top = reactPoint.y-(step/2)*140/88*factor;
				temp.right = reactPoint.x+(step/2)*factor;
				temp.bottom = reactPoint.y+(step/2)*140/88*factor;
				
				BLENDFUNCTION bf;      // structure for alpha blending
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha 
				bf.SourceConstantAlpha = 10;  // opaque (disable constant alpha)

				//pDC->RoundRect(temp.left-2,temp.top-2,temp.right+2,temp.bottom+2,8,8);	
				pDC->AlphaBlend(temp.left+1,temp.top+1,temp.Width()-2,temp.Height()-2,&MemoryDC,
					((stage*4+index)%8)*97,((stage*4+index)/8)*145,88,140,bf); 
				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);
			}
		
	}
	CString Num;
	CFont *pOldFont = pDC->SelectObject(&mSmallFont);
	BackDC.SetTextColor(RGB(255,255,255));
	switch(index)
	{
	case 0:
		Num.Format(_T("새로시작"),index+1);
		break;
	case 1:
		Num.Format(_T("이어하기"),index+1);
		break;
	case 2:
		Num.Format(_T("점수보기"),index+1);
		break;
	case 3:
		Num.Format(_T("끝내기"),index+1);
		break;
	}
	DrawPopText(temp.left,temp.bottom,&BackDC,Num);
	//BackDC.TextOut(temp.left,temp.bottom,Num);
	pDC->SelectObject(pOldFont);
	
	MemoryDC.SelectObject(oldBitmap);
	return 0;
}

void CStoryBoard::DrawPopText(int x,int y,CDC *pDC,CString Message)	
{
	pDC->SetTextColor(RGB(0,0,0));
	pDC->TextOut(x,y-1,Message);
	pDC->TextOut(x,y+1,Message);
	pDC->TextOut(x-1,y,Message);
	pDC->TextOut(x+1,y,Message);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOut(x,y,Message);
}


int CStoryBoard::DrawFace(CDC *pDC)
{
		int i;
		int x,y;
		int index;
		CPoint refPoint;
		CPoint reactPoint;
		CBrush *pOldBrush;
		unsigned int distance;
		static int count = 0;
		double factor;
		CBitmap *oldBitmap;
	
		BackDC.SetBkMode(TRANSPARENT);
		BackDC.SetTextColor(RGB(0,0,0));
		BackDC.SetStretchBltMode(HALFTONE);
		unsigned int step;

		CString Num;
		CBrush GradateBack;
	
		step = mRegion.Width() / 12;
		if (step > 50) step= 50;

		CRect temp;

		if(mMousePoint.x ==0)
			index = 0;
		else
		{
			index = (mRegion.right - mMousePoint.x) / step ;
			index = 4 - index;
		}
		if (index > nItem-1)index = nItem-1;
		if (index < 0) index = 0;
		if(1)
		{
		
			GradateBack.CreateSolidBrush(RGB(140+index*30,140-stage*30,140-index*30));
			///GradateBack.CreateSolidBrush(RGB(30,30,30));
		
			pOldBrush = BackDC.SelectObject(&GradateBack);
		}
		else
		{	
			pOldBrush = BackDC.SelectObject(&BackBrush);
		}	
		BackDC.Rectangle(mRegion.left,mRegion.top,mRegion.right,mRegion.bottom);
		//BackDC.RoundRect(mRegion.left+30,mRegion.top+30,mRegion.right-30,mRegion.bottom-30,50,50);
		BackDC.SelectObject(pOldBrush);
	
		//DrawPopText(mRegion.left,mRegion.top,&BackDC,0,_T("4월 벗꽃화투 花鬪"));
	

		count++;		
		if(nFlower<800)
		{
			nFlower++;
		};
		if(count>400)
		{
			count=0;
			//nFlower=100;
			stage++;
			if(stage>11)stage = 0;
			if(stage ==2)SetDest(pDC,_T("맞고"));
			if(stage ==4)SetDest(pDC,_T("쟁투"));
			if(stage ==6)SetDest(pDC,_T("게임"));
			if(stage ==8)SetDest(pDC,_T("Go!!"));


		}
	#if 1
		oldBitmap=MemoryDC.SelectObject(&curBitmap); // 메모리 DC가 curBitmap을 선택
		BLENDFUNCTION bf;      // structure for alpha blending
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha 
		bf.SourceConstantAlpha = 20;  // opaque (disable constant alpha)
	
		for(i=0;i<nItem;i++)
		{
			BackDC.AlphaBlend((i*200),0,200,320,&MemoryDC,
						((stage*4+i)%8)*97,((stage*4+i)/8)*145,88,140,bf);  	
		}
		MemoryDC.SelectObject(oldBitmap);
	#endif
	#if 1
		for(int i=0;i<nFlower;i++)
		{
			CPen *pOldPen = BackDC.SelectObject(&WhitePen[i%5]);
		//	Flower[i].y = mRegion.CenterPoint().y - rand() % 4 + 1;
		
			BackDC.MoveTo(Flower[i].x,Flower[i].y);
		
			FlowerVelocity[i].y -= rand() % 5 -2;
			FlowerVelocity[i].x += rand() % 5 -2;

			if(stage % 2){
				FlowerVelocity[i].x = (Flower[i].x<FlowerDest[i].x)?FlowerVelocity[i].x+2 : FlowerVelocity[i].x-2;
				FlowerVelocity[i].y = (Flower[i].y<FlowerDest[i].y)?FlowerVelocity[i].y+2 : FlowerVelocity[i].y-2;
			}

			if(mOldMousePoint != mMousePoint){
				FlowerVelocity[i].x = (Flower[i].x<mMousePoint.x)?FlowerVelocity[i].x+1 : FlowerVelocity[i].x-1;
				FlowerVelocity[i].y = (Flower[i].y<mMousePoint.y)?FlowerVelocity[i].y+1 : FlowerVelocity[i].y-1;
				mOldMousePoint = mMousePoint;
			}
	
			if(FlowerVelocity[i].y >4) FlowerVelocity[i].y= 4;
			if(FlowerVelocity[i].x >4) FlowerVelocity[i].x= 4;
		
			if(FlowerVelocity[i].y <-4) FlowerVelocity[i].y= -4;
			if(FlowerVelocity[i].x <-4) FlowerVelocity[i].x= -4;
		

			Flower[i].y += FlowerVelocity[i].y;
			Flower[i].x += FlowerVelocity[i].x;
		
	
			if(Flower[i].y < mRegion.top)Flower[i].y = mRegion.bottom;
			else if(Flower[i].y > mRegion.bottom)Flower[i].y = mRegion.top;
			else if(Flower[i].x<mRegion.left)Flower[i].x   = mRegion.right;
			else if(Flower[i].x>mRegion.right)Flower[i].x   = mRegion.left;
			else BackDC.LineTo(Flower[i].x,Flower[i].y);
 
		
			//BackDC.SetPixel(Flower[i].x,Flower[i].y,RGB(255,255,255));
			BackDC.SelectObject(pOldPen);
		}	
		//int j;
		//for( i=0; i<1000;i++)
		//{
		//	BackDC.SetPixel(FlowerDest[i].x,FlowerDest[i].y,RGB(255,255,254));
		//}

	#endif 

	#if 1
		for(i=0;i<nItem;i++)
		{
			DrawCard(&BackDC,i);
		}

		BackDC.SetTextColor(RGB(255,255,255));
		Num.Format(_T("%d월꽃"),stage+1);
		BackDC.TextOut(mrefPoint[0].x-150,mrefPoint[0].y,Num);

	/*
		BackDC.SetTextColor(RGB(255,255,255));
		Num.Format(_T("%d"),index+1);
		BackDC.TextOut(mrefPoint[0].x,mrefPoint[0].y-200,Num);
	*/
		pDC->BitBlt(mRegion.left,mRegion.top,mRegion.Width(),mRegion.Height(),&BackDC,
		mRegion.left,mRegion.top,SRCCOPY); 
	#endif
		return index;
	
}
int CStoryBoard::DoEnding()
{
	return 0;
}

int CStoryBoard::SetRegion(CRect rect)
{
		mRegion = rect;
		unsigned int step = mRegion.Width() / 12;

		CenterPoint = mRegion.CenterPoint();
		if (step > 50) step= 50;
		int index;
		GForce =100;
		for(index=0;index<nItem;index++)
		{
			CRect temp;
			
			mrefPoint[index].x = mRegion.left+(step*(index*2+1))/2+(rand() % mRegion.Width()-step)-mRegion.Width()/2;
			mrefPoint[index].y = (mRegion.top+2*mRegion.bottom)/3+(rand() % mRegion.Height()-step)-mRegion.Height()/2;
			Velocity[index].x =0;
			Velocity[index].y =0;
			 
		}
		for(int i=0;i<1000;i++)
		{
			
			Flower[i].y = mRegion.top;
			Flower[i].x = rand() % mRegion.Width(); 
			FlowerVelocity[i].x=0;
			FlowerVelocity[i].y=0;
		}	
		nFlower = 0;
			return 0;
}

int CStoryBoard::SetMousePoint(CPoint point)
{	
		mOldMousePoint = mMousePoint;
		mMousePoint = point;
		return 0;
}
#if 0
int CGroundSelect::SetRect(CRect *crect)
{
	
	return 0;
}
#endif 

int CStoryBoard::SetDest(CDC *pDC,CString Message)
{	
	int j;
	int k=0;
	//CString Message;
	
	//Message.Format("화투");
	BackDC.Rectangle(0,0,81,31);
	BackDC.SetTextColor(RGB(0,0,0));
	BackDC.TextOut(0,0,Message);
	int i;
	for(;k<1000;){
		for(i=1;i<100 && k<1000;i++)
		{
			for(j=1;j<40 && k<1000;j++)
			{

				if(BackDC.GetPixel(i,j) == 0)
				{
					FlowerDest[k].x = i*7+100 + rand() % 9 -5;
					FlowerDest[k].y = j*7+100 + rand() % 9 -5;
					k++;
				}
				
			}
		}
	}
	return 0;
}
int CStoryBoard::InitSelectBar(CDC *pDC,CRect *crect)
{
	mpDC = pDC;
	MemoryDC.CreateCompatibleDC(pDC);
	BackDC.CreateCompatibleDC(pDC);
	
	BuffBMP.CreateCompatibleBitmap(pDC, crect->Width(), crect->Height());  
	BackDC.SelectObject(&BuffBMP);

	
	mBaseFont.CreatePointFont(300,_T("-흥수")); 
	mSmallFont.CreatePointFont(130,_T("-흥수"));
	BackDC.SelectObject(&mBaseFont);
	pDC->SelectObject(&mBaseFont);
	SetDest(pDC,_T("화투"));
	return 0;
}