#include <stdio.h>


int safe_add(int lhs_add, int rhs_add) {
  if((((lhs_add>0) && (rhs_add>0) && (lhs_add > (2147483647-rhs_add))) || ((lhs_add<0) && (rhs_add<0) && (lhs_add < (-2147483648-rhs_add))))) { return lhs_add; }
  return lhs_add + rhs_add;
}

int safe_sub(int lhs_sub, int rhs_sub) {
  if(((lhs_sub > 0) && (rhs_sub < 0) && (lhs_sub > 2147483647 + rhs_sub))|| ((lhs_sub < 0) && (rhs_sub > 0) && (lhs_sub < -2147483648 + rhs_sub))) { return lhs_sub; }
  return lhs_sub - rhs_sub;
}

int safe_mul(int lhs_mul, int rhs_mul) {
  if((((lhs_mul > 0) && (rhs_mul > 0) && (lhs_mul > (2147483647 / rhs_mul))) || ((lhs_mul > 0) && (rhs_mul <= 0) && (rhs_mul < (-2147483648 / lhs_mul))) || ((lhs_mul <= 0) && (rhs_mul > 0) && (lhs_mul < (-2147483648 / rhs_mul))) || ((lhs_mul <= 0) && (rhs_mul <= 0) && (lhs_mul != 0) && (rhs_mul < (2147483647 / lhs_mul))))) { return lhs_mul; }
  return lhs_mul * rhs_mul;
}

int safe_div(int lhs_div, int rhs_div) {
  if(((rhs_div == 0) || ((lhs_div == -2147483648) && (rhs_div == (-1))))) { return lhs_div; }
  return lhs_div / rhs_div;
}

int i_func_5(int i_func_5_i_0, int i_func_5_i_1, int i_func_5_i_2, int i_func_5_i_3, int i_func_5_i_4) {
  int i_L86;
  int i_L87;
  int i_L88;
  int i_L96;
  int i_L97;
  int i_L98;
  int i_L99;
  int i_L100;
  int i_L101;
  int i_L89;
  if (i_func_5_i_4 <= 0) {
    return -741417603;
  } else {
    i_func_5_i_4 = i_func_5_i_4 - 1;
  }

  i_L86 = i_func_5_i_0 = (i_func_5_i_1 == i_func_5_i_2);
  i_L87 = i_func_5_i_0 = (i_func_5_i_1 && i_func_5_i_2);
  i_L88 = i_func_5_i_0 = (i_func_5_i_1 / i_func_5_i_2);
  i_L96 = 501809321;
  i_L97 = 1430075599;
  i_L98 = -1833823890;
  i_L99 = -732799814;
  i_L100 = 695379502;
  i_L101 = 839872538;
  i_L89 = 0;
  while ((i_func_5_i_0 = (767181098 / i_func_5_i_1))) {
    int i_L90;
    int i_L91;
    int i_L93;
    int i_L94;
    int i_L95;
    int i_L92;
    i_L90 = i_func_5_i_0 = (i_func_5_i_1 * i_func_5_i_2);
    i_L91 = i_func_5_i_0 = (!i_func_5_i_1);
    i_L93 = 760004880;
    i_L94 = -507976587;
    i_L95 = -424372922;
    i_func_5_i_4 = i_func_5_i_4 - 4;
    i_L92 = i_func_5(i_func_5(i_func_5_i_0, -1084936227, i_func_5_i_1, i_func_5_i_2, i_func_5_i_4), i_func_5(i_func_5_i_3, i_L86, i_L87, -1096182529, i_func_5_i_4), i_func_5(i_L88, i_L90, i_L91, i_L93, i_func_5_i_4), (i_L94 / i_L95), i_func_5_i_4);
    
    i_func_5_i_0 = (i_func_5_i_1 + i_func_5_i_2);
    i_func_5_i_0 = (i_func_5_i_1 != 227323848);
    
    printf("%d\n", i_L90);
    printf("%d\n", i_L91);
    printf("%d\n", i_L93);
    printf("%d\n", i_L94);
    printf("%d\n", i_L95);
    printf("%d\n", i_L92);
  }
  i_func_5_i_4 = i_func_5_i_4 - 4;
  i_func_5(i_func_5(i_func_5_i_0, i_func_5_i_1, i_func_5_i_2, i_func_5_i_3, i_func_5_i_4), i_func_5(i_L86, i_L87, i_L88, 2136864742, i_func_5_i_4), (i_L96 >= i_L97), i_func_5(i_L98, i_L99, i_L100, i_L101, i_func_5_i_4), i_func_5_i_4);
  
  printf("%d\n", i_L86);
  printf("%d\n", i_L87);
  printf("%d\n", i_L88);
  printf("%d\n", i_L96);
  printf("%d\n", i_L97);
  printf("%d\n", i_L98);
  printf("%d\n", i_L99);
  printf("%d\n", i_L100);
  printf("%d\n", i_L101);
  i_func_5_i_4 = i_func_5_i_4 - 3;
  return i_func_5((i_func_5_i_0 || i_func_5_i_1), i_func_5(i_func_5_i_2, i_func_5_i_3, i_L86, -1733593334, i_func_5_i_4), i_func_5(i_L87, i_L88, i_L96, i_L97, i_func_5_i_4), (i_L98 && i_L99), i_func_5_i_4);
}

int i_func_6(int i_func_6_i_0) {
  int i_L103;
  int i_L105;
  int i_L104;
  int i_L102;
  int i_L106;
  int i_L107;
  int i_L108;
  if (i_func_6_i_0 <= 0) {
    return 439941725;
  } else {
    i_func_6_i_0 = i_func_6_i_0 - 1;
  }

  i_func_6_i_0 = i_func_6_i_0 - 1;
  i_L103 = i_func_6(i_func_6_i_0);
  i_L105 = -72686272;
  i_L104 = i_L105;
  i_L102 = i_L103 = i_L104;
  i_func_6_i_0 = i_func_6_i_0 - 1;
  i_L106 = i_func_6(i_func_6_i_0);
  i_L107 = i_L103 = (1120620300 + i_L105);
  i_L108 = 0;
  while (((i_L103 = i_L105)) && (i_L108 < 10)) {
    int i_L109;
    int i_L110;
    int i_L111;
    int i_L115;
    i_L109 = i_L103 = (i_L105 || i_L104);
    i_L110 = i_L103 = (i_L105 < i_L104);
    i_L111 = i_L103 = (-1449520584 || i_L105);
    i_L115 = 0;
    i_L108 = i_L108 + 1;
    i_func_6_i_0 = i_func_6_i_0 - 4;
    {
      int i_L112;
      int i_L113;
      int i_L114;
      i_func_6_i_0 = i_func_6_i_0 - 1;
      i_L112 = i_L103 = i_func_5(826708111, 976163845, i_L105, 1084913285, i_func_6_i_0);
      i_func_6_i_0 = i_func_6_i_0 - 3;
      i_L113 = i_func_5(i_func_5(i_L103, -1500942459, 2101462064, 8080656, i_func_6_i_0), i_func_6(i_func_6_i_0), (i_L105 + i_L104), (i_L102 > i_L106), i_func_6_i_0);
      i_L114 = i_L103 = (i_L105 = i_L104);
      i_L103 = (i_L105 > i_L104);
      
      printf("%d\n", i_L112);
      printf("%d\n", i_L113);
      printf("%d\n", i_L114);
    }
    i_func_6_i_0 = i_func_6_i_0 - 1;
    while (((i_L103 = i_func_6(i_func_6_i_0))) && (i_L115 < 10)) {
      int i_L116;
      int i_L117;
      int i_L118;
      i_func_6_i_0 = i_func_6_i_0 - 1;
      i_L116 = i_func_6(i_func_6_i_0);
      i_L117 = i_L103 = (i_L105 + i_L104);
      i_func_6_i_0 = i_func_6_i_0 - 1;
      i_L118 = i_L103 = i_func_6(i_func_6_i_0);
      i_L115 = i_L115 + 1;
      i_func_6_i_0 = i_func_6_i_0 - 1;
      i_L103 = i_func_5(i_L105, 365261604, i_L104, i_L102, i_func_6_i_0);
      i_L103 = i_L105;
      i_func_6_i_0 = i_func_6_i_0 - 1;
      i_func_5((i_L103 + i_L105), (i_L104 < -142739143), (!i_L102), i_L106, i_func_6_i_0);
      
      printf("%d\n", i_L116);
      printf("%d\n", i_L117);
      printf("%d\n", i_L118);
    }
    
    printf("%d\n", i_L109);
    printf("%d\n", i_L110);
    printf("%d\n", i_L111);
  }
  i_L103 = safe_mul(-850525114, i_L105);
  
  printf("%d\n", i_L103);
  printf("%d\n", i_L105);
  printf("%d\n", i_L104);
  printf("%d\n", i_L102);
  printf("%d\n", i_L106);
  printf("%d\n", i_L107);
  return i_L103 = safe_mul(i_L105, i_L104);
}

int top_caller(int top_caller_i_8) {
  int i_L121;
  int i_L122;
  int i_L120;
  int i_L123;
  int i_L119;
  int i_L124;
  int i_L125;
  if (top_caller_i_8 <= 0) {
    return -1316552579;
  } else {
    top_caller_i_8 = top_caller_i_8 - 1;
  }

  i_L121 = 2087312208;
  i_L122 = 1300624404;
  i_L120 = i_L121 < i_L122;
  i_L123 = -2145814049;
  i_L119 = i_L120 = (-312487024 - i_L123);
  i_L124 = i_L121 = (i_L122 != i_L120);
  i_L125 = i_L121 = (i_L122 >= i_L120);
  i_L121 = (i_L122 > i_L120);
  top_caller_i_8 = top_caller_i_8 - 1;
  i_L121 = top_caller(top_caller_i_8);
  
  printf("%d\n", i_L121);
  printf("%d\n", i_L122);
  printf("%d\n", i_L120);
  printf("%d\n", i_L123);
  printf("%d\n", i_L119);
  printf("%d\n", i_L124);
  printf("%d\n", i_L125);
  top_caller_i_8 = top_caller_i_8 - 1;
  return i_L121 = i_func_6(top_caller_i_8);
}


int main() {
  top_caller(10);
  return 0;
}
