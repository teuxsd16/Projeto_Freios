/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\FUNCAO PERIGO\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void perigo() \\para quando o freio passar da temp. de perda de freio
{
digitalWrite(11, LOW);
digitalWrite(12, LOW);
digitalWrite(13, HIGH); // liga led vermelho
lcd.setCursor(0,1);
lcd.println("FREIO SUPERAQ."); \\Freio superaquecido
tone(7,262,250);
tone(7,294,250);
delay(500);
lcd.setCursor(0,1); \\ faz a tela ficar piscando
lcd.println(" "); \\ apaga o que foi escrito anteriormente para ficar piscando
tone(7,262,250);
tone(7,294,250);
delay(500);
}
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\FUNCAO CUIDADO\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void cuidado() \\ para quando o freio estiver quase no limite
{
digitalWrite(11, LOW);
digitalWrite(12, HIGH); // liga led amarelo
digitalWrite(13, LOW);
lcd.setCursor(0,1);
lcd.println("PRE LIMITE");
tone(7,440,500); // ativa o buzzer numa frequencia 440Hz (Lá) por 500 ms. tone(pino,freq.,tempo em ms)
delay(500);
lcd.setCursor(0,1);
lcd.println("CUIDADO"); \\ muda entre PRE LIMITE e CUIDADO
tone(7,440,500);
delay(500);
}
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\FUNCAO TEMPERATURA MEDIANA\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void normal() \\para quando o feio estiver nem quente nem frio
{
lcd.setCursor(0,1);
lcd.println("FREIO Cond. Nor"); \\ freio em condicao normal
digitalWrite(11, HIGH); // liga led verde
digitalWrite(12, LOW);
digitalWrite(13, LOW);
}
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\FUNCAO TEMPERATURA BAIXA\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void suavedade() \\ para quando o freio nao estiver quente
{
lcd.setCursor(0,1);
lcd.println("Freio Resfriado");
digitalWrite(11, HIGH); // liga led verde
digitalWrite(12, LOW);
digitalWrite(13, LOW);
}
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ CRIACAO DE UMA VARIAVEL PRA LEITURA DE TEMPERATURA \\\\\\\\\\\\\\\\\\\\\*/

Leitura = termopar.readCelsius();
soma = 0;

//calculando média entre os valores
	for (int i=0; i<=4; i++) {
	Leitura = termopar.readCelsius();
	soma = soma + Leitura;
	delay(200);
}

Leitura = soma/5; // media final entre os valores

/* mudanca no display */

	if Leitura >= 0 && Leitura <= 100 { 
	suavedade();
} 

	else if Leitura >= 101 && Leitura <= 449 { 
	normal();
} 

	else if Leitura >= 450 && Leitura <= 599 { 
	cuidado();
} 

	else if Leitura >= 600 { 
	perigo();
}
