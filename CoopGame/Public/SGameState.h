/ /   F i l l   o u t   y o u r   c o p y r i g h t   n o t i c e   i n   t h e   D e s c r i p t i o n   p a g e   o f   P r o j e c t   S e t t i n g s . 
 
 # p r a g m a   o n c e 
 
 # i n c l u d e   " C o r e M i n i m a l . h " 
 # i n c l u d e   " G a m e F r a m e w o r k / G a m e S t a t e B a s e . h " 
 # i n c l u d e   " S G a m e S t a t e . g e n e r a t e d . h " 
 
 
 U E N U M ( B l u e p r i n t T y p e ) 
 e n u m   c l a s s   E W a v e S t a t e   :   u i n t 8 
 { 
 	 W a i t i n g T o S t a r t , 
 
 	 W a v e I n P r o g r e s s , 
 
 	 / /   N o   l o n g e r   s p a w n i n g   n e w   b o t s ,   w a i t i n g   f o r   p l a y e r s   t o   k i l l   r e m a i n i n g   b o t s 
 	 W a i t i n g T o C o m p l e t e , 
 
 	 W a v e C o m p l e t e , 
 
 	 G a m e O v e r , 
 } ; 
 
 U C L A S S ( ) 
 c l a s s   C O O P G A M E _ A P I   A S G a m e S t a t e   :   p u b l i c   A G a m e S t a t e B a s e 
 { 
 	 G E N E R A T E D _ B O D Y ( ) 
 
 p r o t e c t e d : 
 
 	 U F U N C T I O N ( ) 
 	 v o i d   O n R e p _ W a v e S t a t e ( E W a v e S t a t e   O l d S t a t e ) ; 
 
 	 U F U N C T I O N ( B l u e p r i n t I m p l e m e n t a b l e E v e n t ,   C a t e g o r y   =   " G a m e S t a t e " ) 
 	 v o i d   W a v e S t a t e C h a n g e d ( E W a v e S t a t e   N e w S t a t e ,   E W a v e S t a t e   O l d S t a t e ) ; 
 
 	 U P R O P E R T Y ( B l u e p r i n t R e a d O n l y ,   R e p l i c a t e d U s i n g   =   O n R e p _ W a v e S t a t e ,   C a t e g o r y   =   " G a m e S t a t e " ) 
 	 E W a v e S t a t e   W a v e S t a t e ; 
 
 p u b l i c : 
 
 	 v o i d   S e t W a v e S t a t e ( E W a v e S t a t e   N e w S t a t e ) ; 
 	 
 } ; 