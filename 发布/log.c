diff --git a/APP/CS99XX/cs99xx_it.c b/APP/CS99XX/cs99xx_it.c
index b085022..933608d 100644
--- a/APP/CS99XX/cs99xx_it.c
+++ b/APP/CS99XX/cs99xx_it.c
@@ -98,6 +98,8 @@ void init_exit(void)
  * 输入  ：无
  * 输出  ：无
  * 返回  ：无
+ * 记录: 2017.5.13
+ *          增加 中断开关标记 test_flag.exit_sw 0是关 1是开
  */
 void exit_sw(void)
 {
@@ -183,6 +185,8 @@ void exit_sw(void)
 			ARC_INT(DISABLE);	/* 电弧侦测 */
             break;
 	}
+    
+    test_flag.exit_sw = 1;
 }
 
 /*
@@ -191,18 +195,31 @@ void exit_sw(void)
  * 输入  ：无
  * 输出  ：无
  * 返回  ：无
+ * 记录: 2017.5.13
+ *          增加 中断开关标记 test_flag.exit_sw 0是关 1是开
  */
 void exit_off(void)
 {
 	ARC_INT(DISABLE);	/* 关电弧侦测 */
 	STOP_INT(DISABLE);  /* 关闭复位中断 */
 	GFI_INT(DISABLE);   /* 关闭GFI中断 */
+    test_flag.exit_sw = 0;
 }
+/*
+ * 函数名：exit_off_all
+ * 描述  ：关闭全部外部中断
+ * 输入  ：无
+ * 输出  ：无
+ * 返回  ：无
+ * 记录: 2017.5.13
+ *          增加 中断开关标记 test_flag.exit_sw 0是关 1是开
+ */
 void exit_off_all(void)
 {
 	ARC_INT(DISABLE);	/* 关电弧侦测 */
 	SHORT_INT(DISABLE);  /* 关闭复位中断 */
 	GFI_INT(DISABLE);   /* 关闭GFI中断 */
+    test_flag.exit_sw = 0;
 }
 /*
  * 函数名：disable_stop_exit
diff --git a/APP/CS99XX/cs99xx_relay_motion.c b/APP/CS99XX/cs99xx_relay_motion.c
index 8536a32..acc9d89 100644
--- a/APP/CS99XX/cs99xx_relay_motion.c
+++ b/APP/CS99XX/cs99xx_relay_motion.c
@@ -255,6 +255,7 @@ void test_ready(void)
     g_test_time = 0;
     g_dis_time = 0;
     open_test_timer();/* 开定时器 */
+    
     on_schedule_task();/* 打开调度任务 */
 }
 
@@ -529,6 +530,8 @@ void amp_relay_ctrl_off(void)
  * 输入  ：无
  * 输出  ：无
  * 返回  ：无
+ * 记录: 2017.5.13
+ *          增加 close_hv();的调用 在测试停止后要将基准值清0
  */
 void irq_stop_relay_motion(void)
 {
@@ -543,6 +546,7 @@ void irq_stop_relay_motion(void)
     test_flag.test_led_flag = 0;
     close_sine();
     amp_relay_ctrl_off();
+    close_hv();/* 关闭高压基准 */
 }
 
 /*
diff --git a/APP/CS99XX/irq.c b/APP/CS99XX/irq.c
index a71d933..883977e 100644
--- a/APP/CS99XX/irq.c
+++ b/APP/CS99XX/irq.c
@@ -260,14 +260,36 @@ void report_absorptance_res_value(void)
 	}
 }
 /*
+ * 函数名：exit_on
+ * 描述  ：判断启动0.2s后开断路中断
+ * 修改时间:2017.5.13 
+ * 输入  ：无
+ * 输出  ：无
+ * 返回  ：无
+ */
+static void exit_on(void)
+{
+    if(test_flag.exit_sw == 0)
+    {
+        if(g_test_time > 1)
+        {
+            exit_sw();/* 打开外部中断 */
+        }
+    }
+}
+/*
  * 函数名：test_irq
  * 描述  ：测试时钟控制，被调度任务调用
  * 输入  ：无
  * 输出  ：无
  * 返回  ：无
+ * 记录: 2017.5.13
+ *          增加exit_on();函数的调用 判断启动0.2s后开断路中断 
  */
 void test_irq(void)
 {
+    exit_on();//开外部中断
+    
 	/* 当测试时间为0时 测试时间无穷大 */
 	if(0 == tes_t)
 	{
@@ -387,11 +409,11 @@ void test_irq(void)
             open_hv();
         }
         
-		/* 测试时间到时 下降时间为0 */
-		if(0 == fal_t && g_dis_time == tes_t)
-		{
-			close_hv();
-		}
+// 		/* 测试时间到时 下降时间为0 */
+// 		if(0 == fal_t && g_dis_time == tes_t)
+// 		{
+// 			close_hv();
+// 		}
 	}
 	/* 第三阶段 电压下降 */
 	else if(g_test_time <= thr_t)
@@ -452,11 +474,15 @@ void test_irq(void)
  * 输入  ：无
  * 输出  ：无
  * 返回  ：无
+ * 记录: 2017.5.13
+ *          增加exit_on();函数的调用 判断启动0.2s后开断路中断 
  */
 void test_big_cap_irq(void)
 {
     static uint16_t dis_time_bak;
     
+    exit_on();//开外部中断
+    
 	/* 当测试时间为0时 测试时间无穷大 */
 	if(0 == tes_t)
 	{
@@ -690,9 +716,13 @@ void test_big_cap_irq(void)
  * 输入  ：无
  * 输出  ：无
  * 返回  ：无
+ * 记录: 2017.5.13
+ *          增加exit_on();函数的调用 判断启动0.2s后开断路中断 
  */
 void test_g_irq(void)
 {
+    exit_on();//开外部中断
+    
 	/* 当测试时间为0时 测试时间无穷大 */
 	if(0 == tes_t)
 	{
diff --git a/APP/CS99XX/serve_test.c b/APP/CS99XX/serve_test.c
index 6ef2918..bb24997 100644
--- a/APP/CS99XX/serve_test.c
+++ b/APP/CS99XX/serve_test.c
@@ -2739,6 +2739,9 @@ void wait_dc_gr_send_task_idle(void)
  * 输入  ：无
  * 输出  ：无
  * 返回  ：无
+ * 记录: 2017.5.13
+ *          删除了exit_sw();的调用 改在测试启动0.2s后调用exit_sw();开启中断来解决启动
+ *          总是报警的问题
  */
 void startup(void)
 {
@@ -2791,7 +2794,6 @@ void startup(void)
         }
     }
     
-	exit_sw();/* 打开外部中断 */
     plc_signal_cmd(PLC_SIG_TESTING);/* 向PLC发送测试信号 */
     test_flag.judge_err_en = 1;
 }
diff --git a/APP/CS99XX/serve_test.h b/APP/CS99XX/serve_test.h
index 647a7e2..bd34ba4 100644
--- a/APP/CS99XX/serve_test.h
+++ b/APP/CS99XX/serve_test.h
@@ -272,6 +272,7 @@ typedef struct {
     uint32_t vol_rise_step_t;     /* 电压上升阶段时间计数 */
     uint8_t err_once;/* 失败报警只执行一次 */
     uint8_t test_led_flag;/* 测试灯闪烁标记 */
+    uint8_t exit_sw;/* 外部中断开关标记 2017.5.13 */
 }TEST_FLAG;
 
 enum{
