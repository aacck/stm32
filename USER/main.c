#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#define MAX_LENGTH 50 //最大长度
#define true 1
#define false 0
#define pix_width 4
#define UP 3
#define RIGHT 0
#define LEFT 2
#define DOWN 1
int game_over_flag=false;
u32 score=0;
u8 key;
int food_x=80;                                                                                                                                                                                                                                                                                                                          
int food_y=80;
struct
{
	int snake_grid[MAX_LENGTH][2];
	int lenth;
	int direction;
}snake;
//初始化
void init_snake()
{	
	int i;
	snake.lenth=3;
	snake.direction=RIGHT;
	score=0;
	snake.snake_grid[0][0]=48;
	snake.snake_grid[0][1]=48;
	for(i=1;i<snake.lenth;i++)
	{                                                                                                                                                                                                                                                                                        
		snake.snake_grid[i][0]=snake.snake_grid[0][0]-i*(2*pix_width);
		snake.snake_grid[i][1]=snake.snake_grid[0][1];
		
	}
}
//判定是否命中自己身体
int is_hit_body() {
    int i;
    int x = snake.snake_grid[0][0];
    int y = snake.snake_grid[0][1];
    for (i = 1; i < snake.lenth; i++) {
        if (snake.snake_grid[i][0] == x && snake.snake_grid[i][1] == y) {
            return true;
        }
				if(snake.snake_grid[i][0]==0) //优化
				{
					break;
				}
    }
    return false;
}

//判定是否命中边界
int is_hit_boundary() {
    int x = snake.snake_grid[0][0];
    int y = snake.snake_grid[0][1];
    if (x < 14 || x > 246) {
        return true;
    } 
		else if( y < 14 || y > 296){
			return true;
		}
		else {
        return false;
    }
}
//绘制地图
void paint_map()
{
	int x;
			for(x=0;x<pix_width;x++)
		LCD_DrawRectangle(10+x,10+x,250+x,300+x);
}
//绘制头部
void paint_head(int x,int y)
{	
	int pix = pix_width;
	LCD_DrawRectangle(x-pix,y-pix,x+pix,y+pix);
}
int getRandon()
{		
		int min = 24;
    int max = 280;

    int range = max - min; // 计算范围
    int r = rand() % range + min; // 生成随机数
    r = r - r % 8 + 8; // 调整为8的整数倍
	return r;
}
//绘制食物
void paint_food( )
{
	LCD_DrawRectangle(food_x-pix_width,food_y-pix_width,food_x+pix_width,food_y+pix_width);
}
void paint_body(int x,int y)
{
	int pix=pix_width;
	LCD_DrawRectangle(x-pix,y-pix,x+pix,y+pix);
}
//更新蛇身体位置
void update_position(){
	int i;
	for(i=snake.lenth-1;i>0;i--)
		{
			snake.snake_grid[i][0]=snake.snake_grid[i-1][0];
			snake.snake_grid[i][1]=snake.snake_grid[i-1][1];
		}
}
// 更新食物位置
void update_food()
{
	int x,y;
	x=getRandon();
	if(x>=240)
	{
		x=240;
	}
	y=getRandon();
	food_x=x;
	food_y=y;
}
int is_ated_food()
{	
	int x,y;
	x=snake.snake_grid[0][0];
	y=snake.snake_grid[0][1];
	if(x==food_x && y==food_y){
		return true;
	}
	return false;	
}
// 更新移动
void move()
{	
	int ea=false;
	int dr=snake.direction;
	int hit_b=is_hit_boundary();
	int hit_itself=is_hit_body();
	ea=is_ated_food();
	if(ea){
		++snake.lenth;
		score+=10;
		update_food();
	}
	switch(dr)
	{
		case RIGHT:
			update_position();
			snake.snake_grid[0][0]=snake.snake_grid[0][0]+2*pix_width;
			if(hit_b || hit_itself)
				game_over_flag=true;		
		break;
		case DOWN:
			update_position();
			snake.snake_grid[0][1]=snake.snake_grid[0][1]+2*pix_width;
			if(hit_b  || hit_itself)
				game_over_flag=true;
			break;
		case LEFT:
			update_position();
			snake.snake_grid[0][0]=snake.snake_grid[0][0]-2*pix_width;
			if(hit_b || hit_itself)
				game_over_flag=true;
			break;
		case UP:
			update_position();
			snake.snake_grid[0][1]=snake.snake_grid[0][1]-2*pix_width;
			if(hit_b || hit_itself)
				game_over_flag=true;
		break ;
	}
	
}
void paint_menu()
{
	
	char l[10];
	char s[10];
	sprintf(l, "%d", snake.lenth);
	sprintf(s, "%d", score);
	LCD_ShowString(30,320,50,16,16,"lenth:");
	LCD_ShowString(80,320,50,16,16,l);
	LCD_ShowString(30,360,50,16,16,"score:");
	LCD_ShowString(80,360,50,16,16,s);
}
//刷新界面
void refresh(int grid[][2])
{	
	int i;
	LCD_Clear(WHITE);
	paint_map();
	paint_head(grid[0][0],grid[0][1]);
	for(i=1;i<snake.lenth;i++)
	{
		paint_body(grid[i][0],grid[i][1]);
	}
	paint_food();
	paint_menu();
}
// 检测按键
void detected_key()
{
				key=KEY_Scan(1);
			switch(key)
			{
				case KEY0_PRES:
					if(snake.direction!=LEFT)
						snake.direction=RIGHT;
					break;
				case KEY1_PRES:
					if(snake.direction!=UP)
						snake.direction=DOWN;
					break;
				case KEY2_PRES:
					if(snake.direction!=RIGHT)
						snake.direction=LEFT;
					break;
				case WKUP_PRES:
					if(snake.direction!=DOWN)
					snake.direction=UP;
					break;
			}
}
void victory(){
while(1){
			LCD_Clear(WHITE);
			key=KEY_Scan(1);
			LCD_ShowString(60,200,200,16,16,"you had successed");
			LCD_ShowString(100,200,200,16,16,"press reset key to restart");
			delay_ms(500);
		}
}
int main(void)
{
    Stm32_Clock_Init(360,25,2,8);   
    delay_init(180);                
    uart_init(115200);              //
    LED_Init();                     // 
    KEY_Init();                     //
    SDRAM_Init();                   //
    LCD_Init();                     //LCD
		init_snake();
		srand(20102);
		while(1){
			LCD_Clear(WHITE);
			key=KEY_Scan(1);
			LCD_ShowString(30,200,200,16,16,"Press any key to start");
			if(key){
				break;
			}
			delay_ms(500);
		}
		while(game_over_flag==false)
		{	
			detected_key();
			refresh(snake.snake_grid);
			delay_ms(500);
			detected_key();
			move();
			if(snake.lenth>=MAX_LENGTH)
			{
				victory();
			}
		}
		while(1){
			LCD_Clear(WHITE);
			key=KEY_Scan(1);
			LCD_ShowString(60,200,200,16,16,"game over");
			delay_ms(500);
		}
		
}