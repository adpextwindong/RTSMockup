#include "stdafx.h"//VS2010 Requirement
#include "Macros.h"

using namespace std;
enum Directions {Up,Down,Left,DLeft,DULeft,Right,DRight,DURight};
	
unsigned int currentx,currenty,targetx,targety=0;
int tf,tg,th,ti,tx,ty,f,g,h=0;
std::vector<int> dir; 

bool openarray[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP];
bool closedarray[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP];

void Astar()
{
	openarray[currentx][currenty]=true;
		for(int i=-1;i<=1;i++)
		{
			for(int j=-1;j<=1;j++)//not sure if this is the correct iteration
			{
				if(i==0&&j==0)
				{
					continue;
				}
				tempx=currentx+j;
				tempy=currenty+i;
				if(closedarray[tempx][tempy]==true)
				{
					continue;
				}else{
					openarray[tempx][tempy]=true;
					//currentx,currenty = parent of this tile;
					th = abs(tempx-targetx) + abs(tempy-targety);
					tg=tg;
					tf=tg+th;
					if(tf<f)
					{
						f=tf;
						ti = i;
						tj = j;
						tx = tempx;
						ty = tempy;
					}
					//check if this score is less than
				}
				
			}
			
			
		}
		if(ti==-1&&tj==-1)
		{
			dir.pushback(DLeft);
		}else if(ti==0&&tj==-1)
		{
			dir.pushback(Left);
		}else if(ti==1&&tj==-1)
		{
			dir.pushback(DULeft);
		}else if(ti==-1&&tj==0)
		{
			dir.pushback(Down);
		}else if(ti==1&&tj==0)
		{
			dir.pushback(Up);
		}else if(ti==-1&&tj==1)
		{
			dir.pushback(DLeft);
		}else if(ti==0&&tj==1)
		{
			dir.pushback(Left);
		}else if(ti==1&&tj==1)
		{
			dir.pushback(DULeft);
		}
		currentx=tx;
		currenty=ty;
}

int _tmain(int argc, _TCHAR* argv[])
{

	while(currentx!=targetx&&currenty!=targety)
	{
		Astar(currentx,currenty);
	}
	
	for( vector<enum> const_iterator i = dir.begin(); i != dir.end(); ++i)
    cout << *i << ' ';

}