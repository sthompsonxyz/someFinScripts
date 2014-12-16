//+------------------------------------------------------------------+
//|                                                         MTF2.mq5 |
//|                       Copyright 2013, Sammysamsam Software Corp. |
//|                                       http://www.sammysamsam.com |
//+------------------------------------------------------------------+
#property copyright "Sammysamsam"
#property link      "http://www.sammysamsam.com"
#property version   "0.0.01"
#property indicator_separate_window
#property indicator_buffers 12
#property indicator_plots 6
#property indicator_type1 DRAW_COLOR_ARROW
#property indicator_color1 clrBlue,clrRed,clrGreen 
#property indicator_style1 STYLE_SOLID
#property indicator_width1 5
#property indicator_type2 DRAW_COLOR_ARROW
#property indicator_color2 clrBlue,clrRed,clrGreen
#property indicator_style2 STYLE_SOLID
#property indicator_width2 5
#property indicator_type3 DRAW_COLOR_ARROW
#property indicator_color3 clrBlue,clrRed,clrGreen
#property indicator_style3 STYLE_SOLID
#property indicator_width3 5
#property indicator_type4 DRAW_COLOR_ARROW
#property indicator_color4 clrBlue,clrRed,clrGreen
#property indicator_style4 STYLE_SOLID
#property indicator_width4 5
#property indicator_type5 DRAW_COLOR_ARROW
#property indicator_color5 clrBlue,clrRed,clrGreen 
#property indicator_style5 STYLE_SOLID
#property indicator_width5 5
#property indicator_type6 DRAW_COLOR_ARROW
#property indicator_color6 clrBlue,clrRed,clrGreen
#property indicator_style6 STYLE_SOLID
#property indicator_width6 5


//globals
int D1offset=1439;
int H1offset=59;
int M30offset=29;
int M15offset=14;
int M5offset=4;

//indicator buffers
double indD1[];
double colors0[];
double indH1[];
double colors1[];
double indM30[];
double colors2[];
double indM15[];
double colors3[];
double indM5[];
double colors4[];
double indM1[];
double colors5[];

input ushort code = 159;

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- indicator buffers mapping
   SetIndexBuffer(0,indD1,INDICATOR_DATA);
   PlotIndexSetInteger(0,PLOT_ARROW,code);
   PlotIndexSetInteger(0,PLOT_ARROW_SHIFT,0);
   SetIndexBuffer(1,colors0,INDICATOR_COLOR_INDEX);

   SetIndexBuffer(2,indH1,INDICATOR_DATA);
   PlotIndexSetInteger(2,PLOT_ARROW,code);
   PlotIndexSetInteger(2,PLOT_ARROW_SHIFT,0);
   SetIndexBuffer(3,colors1,INDICATOR_COLOR_INDEX);
   
   SetIndexBuffer(4,indM30,INDICATOR_DATA);
   PlotIndexSetInteger(4,PLOT_ARROW,code);
   PlotIndexSetInteger(4,PLOT_ARROW_SHIFT,0);
   SetIndexBuffer(5,colors2,INDICATOR_COLOR_INDEX);
   
   SetIndexBuffer(6,indM15,INDICATOR_DATA);
   PlotIndexSetInteger(6,PLOT_ARROW,code);
   PlotIndexSetInteger(6,PLOT_ARROW_SHIFT,0);
   SetIndexBuffer(7,colors3,INDICATOR_COLOR_INDEX);
   
   SetIndexBuffer(8,indM5,INDICATOR_DATA);
   PlotIndexSetInteger(8,PLOT_ARROW,code);
   PlotIndexSetInteger(8,PLOT_ARROW_SHIFT,0);
   SetIndexBuffer(9,colors4,INDICATOR_COLOR_INDEX);
   
   SetIndexBuffer(10,indM1,INDICATOR_DATA);
   PlotIndexSetInteger(10,PLOT_ARROW,code);
   PlotIndexSetInteger(10,PLOT_ARROW_SHIFT,0);
   SetIndexBuffer(11,colors5,INDICATOR_COLOR_INDEX);
   
//---
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//---
   
   //set each of the MTF indicator lines in turn
   if(rates_total > D1offset)
   {
      //set D1
      int i=D1offset;
      while(i<rates_total)
      {
         indD1[i]=0.995;
         if(open[i-D1offset] < close[i])
         {
            colors0[i] = 2;
         }
         else if(open[i-D1offset] > close[i])
         {
            colors0[i] = 1;
         }
         else
         {
            colors0[i] = 0;
         }
         i++;
      }
   
      //set H1
      i=H1offset;
      while(i<rates_total)
      {
   
         indH1[i] = 1;
         if(open[i-H1offset] < close[i])
         {
            colors1[i]=2;
         }
         else if(open[i-H1offset] > close[i])
         {
            colors1[i]=1;
         }
         else
         {
            colors1[i]=0;
         }
      i++;
      }
      
      //set M30
      i=M30offset;
      while(i<rates_total)
      {
         indM30[i]=1.005;
         if(open[i-M30offset] < close[i])
         {
            colors2[i]=2;
         }
         else if(open[i-M30offset] > close[i])
         {
            colors2[i]=1;
         }
         else
         {
            colors2[i]=0;
         }
         i++;
      }
   
      //set M15
      i=M15offset;
      while(i<rates_total)
      {
         indM15[i]=1.01;
         if(open[i-M15offset] < close[i])
         {
            colors3[i] = 2;
         }
         else if(open[i-M15offset] > close[i])
         {
            colors3[i] = 1;
         }
         else
         {
            colors3[i] = 0;
         }
         i++;
      }
      
      //set M5
      i=M5offset;
      while(i<rates_total)
      {
         indM5[i]=1.015;
         if(open[i-M5offset] < close[i])
         {
            colors4[i] = 2;
         }
         else if(open[i-M5offset] > close[i])
         {
            colors4[i] = 1;
         }
         else
         {
            colors4[i] = 0;
         }
         i++;
      }
      
      //set M1
      i=0;
      while(i<rates_total)
      {
         indM1[i] = 1.02;
         if(open[i] < close[i])
         {
            colors5[i] = 2;
         }
         else if(open[i] > close[i])
         {
            colors5[i] = 1;
         }
         else
         {
            colors5[i] = 0;
         }
         i++;
      }
   }
//--- return value of prev_calculated for next call
   return(rates_total);
  }
//+------------------------------------------------------------------+
//| TradeTransaction function                                        |
//+------------------------------------------------------------------+
void OnTradeTransaction(const MqlTradeTransaction& trans,
                        const MqlTradeRequest& request,
                        const MqlTradeResult& result)
  {
//---


  
  }
//+------------------------------------------------------------------+
