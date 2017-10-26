diff --git a/APP/CS99XX/cs99xx_it.c b/APP/CS99XX/cs99xx_it.c
index b085022..933608d 100644
--- a/APP/CS99XX/cs99xx_it.c
+++ b/APP/CS99XX/cs99xx_it.c
@@ -98,6 +98,8 @@ void init_exit(void)
  * ����  ����
  * ���  ����
  * ����  ����
+ * ��¼: 2017.5.13
+ *          ���� �жϿ��ر�� test_flag.exit_sw 0�ǹ� 1�ǿ�
  */
 void exit_sw(void)
 {
@@ -183,6 +185,8 @@ void exit_sw(void)
 			ARC_INT(DISABLE);	/* �绡��� */
             break;
 	}
+    
+    test_flag.exit_sw = 1;
 }
 
 /*
@@ -191,18 +195,31 @@ void exit_sw(void)
  * ����  ����
  * ���  ����
  * ����  ����
+ * ��¼: 2017.5.13
+ *          ���� �жϿ��ر�� test_flag.exit_sw 0�ǹ� 1�ǿ�
  */
 void exit_off(void)
 {
 	ARC_INT(DISABLE);	/* �ص绡��� */
 	STOP_INT(DISABLE);  /* �رո�λ�ж� */
 	GFI_INT(DISABLE);   /* �ر�GFI�ж� */
+    test_flag.exit_sw = 0;
 }
+/*
+ * ��������exit_off_all
+ * ����  ���ر�ȫ���ⲿ�ж�
+ * ����  ����
+ * ���  ����
+ * ����  ����
+ * ��¼: 2017.5.13
+ *          ���� �жϿ��ر�� test_flag.exit_sw 0�ǹ� 1�ǿ�
+ */
 void exit_off_all(void)
 {
 	ARC_INT(DISABLE);	/* �ص绡��� */
 	SHORT_INT(DISABLE);  /* �رո�λ�ж� */
 	GFI_INT(DISABLE);   /* �ر�GFI�ж� */
+    test_flag.exit_sw = 0;
 }
 /*
  * ��������disable_stop_exit
diff --git a/APP/CS99XX/cs99xx_relay_motion.c b/APP/CS99XX/cs99xx_relay_motion.c
index 8536a32..acc9d89 100644
--- a/APP/CS99XX/cs99xx_relay_motion.c
+++ b/APP/CS99XX/cs99xx_relay_motion.c
@@ -255,6 +255,7 @@ void test_ready(void)
     g_test_time = 0;
     g_dis_time = 0;
     open_test_timer();/* ����ʱ�� */
+    
     on_schedule_task();/* �򿪵������� */
 }
 
@@ -529,6 +530,8 @@ void amp_relay_ctrl_off(void)
  * ����  ����
  * ���  ����
  * ����  ����
+ * ��¼: 2017.5.13
+ *          ���� close_hv();�ĵ��� �ڲ���ֹͣ��Ҫ����׼ֵ��0
  */
 void irq_stop_relay_motion(void)
 {
@@ -543,6 +546,7 @@ void irq_stop_relay_motion(void)
     test_flag.test_led_flag = 0;
     close_sine();
     amp_relay_ctrl_off();
+    close_hv();/* �رո�ѹ��׼ */
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
+ * ��������exit_on
+ * ����  ���ж�����0.2s�󿪶�·�ж�
+ * �޸�ʱ��:2017.5.13 
+ * ����  ����
+ * ���  ����
+ * ����  ����
+ */
+static void exit_on(void)
+{
+    if(test_flag.exit_sw == 0)
+    {
+        if(g_test_time > 1)
+        {
+            exit_sw();/* ���ⲿ�ж� */
+        }
+    }
+}
+/*
  * ��������test_irq
  * ����  ������ʱ�ӿ��ƣ��������������
  * ����  ����
  * ���  ����
  * ����  ����
+ * ��¼: 2017.5.13
+ *          ����exit_on();�����ĵ��� �ж�����0.2s�󿪶�·�ж� 
  */
 void test_irq(void)
 {
+    exit_on();//���ⲿ�ж�
+    
 	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
 	if(0 == tes_t)
 	{
@@ -387,11 +409,11 @@ void test_irq(void)
             open_hv();
         }
         
-		/* ����ʱ�䵽ʱ �½�ʱ��Ϊ0 */
-		if(0 == fal_t && g_dis_time == tes_t)
-		{
-			close_hv();
-		}
+// 		/* ����ʱ�䵽ʱ �½�ʱ��Ϊ0 */
+// 		if(0 == fal_t && g_dis_time == tes_t)
+// 		{
+// 			close_hv();
+// 		}
 	}
 	/* �����׶� ��ѹ�½� */
 	else if(g_test_time <= thr_t)
@@ -452,11 +474,15 @@ void test_irq(void)
  * ����  ����
  * ���  ����
  * ����  ����
+ * ��¼: 2017.5.13
+ *          ����exit_on();�����ĵ��� �ж�����0.2s�󿪶�·�ж� 
  */
 void test_big_cap_irq(void)
 {
     static uint16_t dis_time_bak;
     
+    exit_on();//���ⲿ�ж�
+    
 	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
 	if(0 == tes_t)
 	{
@@ -690,9 +716,13 @@ void test_big_cap_irq(void)
  * ����  ����
  * ���  ����
  * ����  ����
+ * ��¼: 2017.5.13
+ *          ����exit_on();�����ĵ��� �ж�����0.2s�󿪶�·�ж� 
  */
 void test_g_irq(void)
 {
+    exit_on();//���ⲿ�ж�
+    
 	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
 	if(0 == tes_t)
 	{
diff --git a/APP/CS99XX/serve_test.c b/APP/CS99XX/serve_test.c
index 6ef2918..bb24997 100644
--- a/APP/CS99XX/serve_test.c
+++ b/APP/CS99XX/serve_test.c
@@ -2739,6 +2739,9 @@ void wait_dc_gr_send_task_idle(void)
  * ����  ����
  * ���  ����
  * ����  ����
+ * ��¼: 2017.5.13
+ *          ɾ����exit_sw();�ĵ��� ���ڲ�������0.2s�����exit_sw();�����ж����������
+ *          ���Ǳ���������
  */
 void startup(void)
 {
@@ -2791,7 +2794,6 @@ void startup(void)
         }
     }
     
-	exit_sw();/* ���ⲿ�ж� */
     plc_signal_cmd(PLC_SIG_TESTING);/* ��PLC���Ͳ����ź� */
     test_flag.judge_err_en = 1;
 }
diff --git a/APP/CS99XX/serve_test.h b/APP/CS99XX/serve_test.h
index 647a7e2..bd34ba4 100644
--- a/APP/CS99XX/serve_test.h
+++ b/APP/CS99XX/serve_test.h
@@ -272,6 +272,7 @@ typedef struct {
     uint32_t vol_rise_step_t;     /* ��ѹ�����׶�ʱ����� */
     uint8_t err_once;/* ʧ�ܱ���ִֻ��һ�� */
     uint8_t test_led_flag;/* ���Ե���˸��� */
+    uint8_t exit_sw;/* �ⲿ�жϿ��ر�� 2017.5.13 */
 }TEST_FLAG;
 
 enum{
