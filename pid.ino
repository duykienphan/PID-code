float kp = 20, ki = 0.015, kd = 200; // Thiết lập giá trị PID, các giá trị này sẽ không còn đúng khi pin sụt
float error = 0 , p = 0 , i = 0 , d = 0, pid = 0;
float pre_e  = 0;
int ss[5] = {0, 0, 0, 0, 0};
int giatribandau = 100; //Tốc độ ban đầu của xe
float pidphai , pidtrai;

// Cấu hình chân IO
#define in1 6
#define in2 7
#define in3 5
#define in4 4
#define ena 11
#define enb 10

void setup() {
  // put your setup code here, to run once:
  pinMode(ena , OUTPUT);
  pinMode(enb , OUTPUT);
  pinMode(in1 , OUTPUT);
  pinMode(in2 , OUTPUT);
  pinMode(in3 , OUTPUT);
  pinMode(in4 , OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ss_read();
  cal();
  motor();
}
void ss_read() {
  int a = 1; //Cảm biến không nhận được line
  int b = 0; //Cảm biến nhận được line

  //Đọc giá trị cảm biến
  ss[0] = digitalRead(A0);
  ss[1] = digitalRead(A1);
  ss[2] = digitalRead(A2);
  ss[3] = digitalRead(A3);
  ss[4] = digitalRead(A4);

  // Thiết lập các trường hợp
  if ((ss[0] == a) && (ss[1] == a) && (ss[2] == b) && (ss[3] == b) && (ss[4] == b)){ // Trường hợp đặc biệt (Không quan trọng, có thể xoá trường hợp này)
    error = -4;
    digitalWrite(in1 , LOW);
    digitalWrite(in2 , LOW);
    digitalWrite(in3 , LOW);
    digitalWrite(in4 , LOW);
    delay(500);
    digitalWrite(in1 , LOW);
    digitalWrite(in2 , HIGH);
    digitalWrite(in3 , LOW);
    digitalWrite(in4 , HIGH);
    analogWrite(ena, 30);
    analogWrite(enb, 100);
    delay(500);
    digitalWrite(in1 , LOW);
    digitalWrite(in2 , LOW);
    digitalWrite(in3 , LOW);
    digitalWrite(in4 , LOW);
    delay(500);
    digitalWrite(in1 , LOW);
    digitalWrite(in2 , HIGH);
    digitalWrite(in3 , LOW);
    digitalWrite(in4 , HIGH);
    analogWrite(ena, 100);
    analogWrite(enb, 100);
    delay(50);
    error = -5;
  }
  if ((ss[0] == a) && (ss[1] == a) && (ss[2] == a) && (ss[3] == a) && (ss[4] == b)){ 
    error = -4;
  }
  if ((ss[0] == a) && (ss[1] == a) && (ss[2] == a) && (ss[3] == b) && (ss[4] == b)){ 
    error = -3;
  }
  if ((ss[0] == a) && (ss[1] == a) && (ss[2] == a) && (ss[3] == b) && (ss[4] == a)){ 
    error = -2;
  }
  if ((ss[0] == a) && (ss[1] == a) && (ss[2] == b) && (ss[3] == b) && (ss[4] == a)){ 
    error = -1;
  }
  if ((ss[0] == a) && (ss[1] == a) && (ss[2] == b) && (ss[3] == a) && (ss[4] == a)){
    error = 0; //Mắt giữa nhận => Đi thẳng
  }
  if ((ss[0] == a) && (ss[1] == b) && (ss[2] == b) && (ss[3] == a) && (ss[4] == a)){ 
    error = 1;
  }
  if ((ss[0] == a) && (ss[1] == b) && (ss[2] == a) && (ss[3] == a) && (ss[4] == a)){ 
    error = 2;
  }
  if ((ss[0] == b) && (ss[1] == b) && (ss[2] == a) && (ss[3] == a) && (ss[4] == a)){ 
    error = 3;
  }
  if ((ss[0] == b) && (ss[1] == a) && (ss[2] == a) && (ss[3] == a) && (ss[4] == a)){ 
    error = 4;
  }
  if ((ss[0] == a) && (ss[1] == a) && (ss[2] == a) && (ss[3] == a) && (ss[4] == a)){ 
    error = pre_e; //Khi không nhận được line sẽ nhận lệnh của error trước đó
  }
}


void cal(){ //Công thức tính toán thông số PID
  p = error;
  i = i + error;
  d = error - pre_e;
  pid = (kp * p) + (ki * i) + (kd * d);
  pre_e = error;
}
 
void motor() { //Điều khiển động cơ
  //Động cơ đi thẳng, tuỳ vào xung cấp cho chân ena và enb để điều hướng trái phải
  digitalWrite(in1 , LOW);
  digitalWrite(in2 , HIGH);
  digitalWrite(in3 , LOW);
  digitalWrite(in4 , HIGH);

  //Công thức tính giá trị xung cấp cho chân ena và enb
  int pid_phai = giatribandau - pid;
  int pid_trai = giatribandau + pid;

  //Ràng buộc 2 giá trị pid_trai va pid_phai trong khoảng 0->100
  pid_phai = constrain(pid_phai, 0, 100);
  pid_trai = constrain(pid_trai, 0, 100);
  
  //Xuất xung
  analogWrite(ena, pid_trai);
  analogWrite(enb, pid_phai);
  
  //Serial.print(ss[0]);
  //Serial.print(ss[1]);
  //Serial.print(ss[2]);
  //Serial.print(ss[3]);
  //Serial.print(ss[4]);
  //Serial.println(error);
  //Serial.print(pid_phai);
  //Serial.print("_");
  //Serial.println(pid_trai);
}
