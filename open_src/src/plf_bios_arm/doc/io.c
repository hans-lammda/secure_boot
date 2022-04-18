#define GPIO_BASE         0x6400

#define SCL                 12
#define SDA                 13

mod()
{
  /* SCL */
  outl(SCL,  GPIO_BASE + 0x20);
  /* 45 */
  outl(0x45, GPIO_BASE + 0x24);
  /* END */
}


