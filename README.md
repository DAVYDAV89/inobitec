# inobitec

Необходимо разработать простую программную систему из двух
приложений на базе Qt Framework (С++) для мониторинга регулярного
непрерывного сигнала с воображаемого источника

Приложение 1 - Эмулятор (сервер)
Производит непрерывную генерацию указанного сигнала на программном
уровне и обеспечивает его передачу TCP\IP транспортом по произвольному
протоколу.
Приложение 2 - Регистратор (клиент)
Принимает и отображает в произвольной форме сигнал, посланный
Эмулятором, с применением встроенной Qt функциональности.
Это может быть текстовое или графическое представление.