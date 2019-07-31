#include <Servo.h>

Servo garra;
Servo base;
int base_p;

#define vir_esq 58
#define vir 60
#define rot 31
#define del 2000
#define ang 76

void setup()
{
  // Define Garra no pino 6
  garra.attach(6);
  // Define Base no pino 11
  base.attach(11);
  // Define clock do Arduino
  Serial.begin(9600);
  // Inicializa Garra e Base
  inicializa();
}

void loop()
{
  if (Serial.available())
  {
    // Recebe comando a ser executado
    char char_comando = Serial.read();
    int comando = Serial.parseInt();
    Serial.println(comando);
    switch(comando)
    {
      case 1: // vai do up pro front // 5s
        vira(vir);
        break;
      case 2: // vai do front pro down // 5s
        vira(vir);
        break;
      case 3: // vai do down pro back // 5s
        vira(vir);
        break;
      case 4: // vai do back pro right // 9s
        vira(vir);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        break;
      case 5: // vai do right pro left // 7s
        vira_base_esquerda();
        vira(vir_esq);
        vira(vir_esq);
        vira_base_centro();
        break;
      case 6: // retorna do left pro up // 12s
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        vira(vir);
        vira(vir);
        break;
      case 7: //U // 35s
        vira(vir);
        vira(vir);
        rotaciona(rot);
        
        vira(vir);
        vira(vir);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        
        vira(vir);
        vira(vir);
        break;
      case 8: //U' // 20s
        vira(vir);
        vira(vir);
        rotacionaInv(rot);
        
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        
        vira(vir);
        break;
      case 9: //F // 35s
        vira(vir);
        vira(vir);
        vira(vir);
        rotaciona(rot);
        
        vira(vir);
        vira(vir);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        
        vira(vir);
        break;
      case 10: //F' // 20s
        vira(vir);
        vira(vir);
        vira(vir);
        rotacionaInv(rot);

        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();

        break;
      case 11: //D // 31s
        rotaciona(rot);

        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira(vir_esq);
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        vira(vir);
        vira(vir);
        
        break;
      case 12: //D' // 20s
        rotacionaInv(rot);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        vira(vir);
        vira(vir);
        break;
      case 13: //B // 27s
        vira(vir);
        rotaciona(rot);

        vira(vir);
        vira(vir);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        break;
      case 14: //B' // 19s
        vira(vir);
        rotacionaInv(rot);

        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        
        vira(vir);
        vira(vir);
        break;
      case 15: //R // 39s
        vira_base_esquerda();
        vira(vir_esq);
        vira(vir_esq);
        vira(vir_esq);
        vira_base_centro();
        rotaciona(rot);
        
        vira(vir);
        vira(vir);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        break;
      case 16: //R' // 29s
        vira_base_esquerda();
        vira(vir_esq);
        vira(vir_esq);
        vira(vir_esq);
        vira_base_centro();
        rotacionaInv(rot);

        vira(vir);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira(vir_esq);
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        break;
      case 17: //L // 39s
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        rotaciona(rot);

        vira(vir);
        vira(vir);
        vira(vir);
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        
        vira_base_esquerda();
        vira(vir_esq);
        vira(vir_esq);
        vira(vir_esq);
        vira_base_centro();
        break;
      case 18: //L'// 21s
        vira_base_esquerda();
        vira(vir_esq);
        vira_base_centro();
        rotacionaInv(rot);
        
        vira_base_esquerda();
        vira(vir_esq);
        vira(vir_esq);
        vira(vir_esq);
        vira_base_centro();
        vira(vir);
        break;
    }
  }
}

void vira_base_esquerda()
{
  base.write(5);
  delay(del); 
}

void vira_base_centro()
{
  base.write(90);
  delay(del);
}


void inicializa()
{
  garra.write(0);
  base.write(90);
  base_p = 90;
  delay(del);
}

void vira(int n)
{
  garra.write(n);
  delay(del);
  garra.write(0);
  delay(del);
}

void rotacionaInv(int n)
{
  base.write(180);
  delay(del);
  garra.write(n);
  delay(del);
  base.write(ang);
  base_p = 0;
  delay(del);
  garra.write(0);
  delay(del);
  base.write(90);
  base_p = 90;
  delay(del);
}

void rotaciona(int n)
{
  base.write(180);
  delay(del);
  garra.write(n);
  delay(del);
  base.write(ang);
  base_p = 0;
  delay(del);
  garra.write(0);
  delay(del);
  base.write(180);
  delay(del);
  garra.write(n);
  delay(del);
  base.write(ang);
  base_p = 0;
  delay(del);
  garra.write(0);
  delay(del);
  base.write(180);
  delay(del);
  garra.write(n);
  delay(del);
  base.write(ang);
  base_p = 0;
  delay(del);
  garra.write(0);
  delay(del);
  base.write(90);
  base_p = 90;
  delay(del);
}




