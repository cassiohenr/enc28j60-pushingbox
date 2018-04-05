#include <EtherCard.h>

static byte mymac[] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};   

boolean alfa = true;
 
char website[] PROGMEM = "api.pushingbox.com";
byte Ethernet::buffer[700];
Stash stash;

int vccsensor = 5;
int gndsensor = 6;;
int enviar=0;

int lm35 = A0;

void setup () 
{
  Serial.begin(9600);
  pinMode(vccsensor, OUTPUT);
  pinMode(gndsensor, OUTPUT);
  pinMode(enviar, OUTPUT);  

  
  if(alfa)
  {
  Serial.println("\nInicializando...");
  Serial.println("\nObtendo DHCP e DNS...");
  }
  
  if(ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0) 
  {
    if(alfa){Serial.println( "Falha para acessar -> Controlador Ethernet");}
  }
  
  while(!ether.dhcpSetup())
  {
    if(alfa)
    {
      Serial.println("Falha em DHCP. Não foi possível obter o endereço de IP. Tentar novamente...");
    }
  }
  
  if(alfa)
  {
    ether.printIp("IP: ", ether.myip);
    ether.printIp("GW IP: ", ether.gwip);
    ether.printIp("DNS IP: ", ether.dnsip);
  }

  if (!ether.dnsLookup(website))
  {
    if(alfa){Serial.println("Falha - DNS");}
  }

  if(alfa)
  {
    ether.printIp("Servidor: ", ether.hisip);
  }

}

void loop () 
{
   
   digitalWrite(vccsensor, HIGH);
   digitalWrite(gndsensor, LOW);
   char mydata[33];
   
   int temp = ( analogRead(lm35) * 0.4887585532746823069403714565);
   
   itoa(temp, mydata, 10);
    
   ether.packetLoop(ether.packetReceive());
 
//-------------------------------------------------------------------------------------   
   enviar=enviar+1;
   if (enviar == 1) 
      {
        if(alfa){Serial.println("Enviando...");}
        Serial.println(mydata);
        ether.browseUrl(PSTR("/pushingbox?devid=SUACHAVE&data="), mydata, website, retorno);
        enviar=1;
        delay(500); 
       }
//--------------------------------------------------------------------------------------       
}

static void retorno (byte status, word off, word len) 
{
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}
