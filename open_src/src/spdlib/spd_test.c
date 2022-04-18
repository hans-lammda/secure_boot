#include <plf/platform.h>
#include <plf/memory.h>
#include <plf/file.h>
#include <spd/spd.h>

#define BUNDLE_MAGIC 0xAA6400F1
#define BUNDLE_ID_LENGTH 31

struct bundle_data {
    uint32_t size;
    uint32_t offset;
    uint32_t flags;
    uint32_t time;
    char id[BUNDLE_ID_LENGTH + 1];
};

struct bundle_hdr {
    uint32_t magic;
    uint32_t size;
    uint32_t count;
    uint32_t flags;
    uint32_t time;
    struct bundle_data data[];
};



#define DBG_LEVEL 1 

const char main_version[] = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/spdlib/branches/haspoc/juno/spdlib/test_dir/spd_test.c $ $Revision: 26514 $";



int main (int argc, char *argv[])
{
 char *src, *dst;
 int key, *buf , i,rc , cert_len ,sig_len, *p; 
 unsigned long   len, cert_start, sig_start;

 struct sobject debug_print, *tp,  *sp, *end , *trust_root;
 void *vp; 
 file_io temp,out; 
 struct bundle_hdr *b_hdr; 



 if (argc < 3) {
   printf ("usage: spd_verify <inile> <key>\n");
   exit(1); 
 }

 key = atoi(argv[2]); 

 tp = get_root_pk_hash(); 

 install_key(&trust_root, tp); 
 
 temp =  plf_load_file(argv[1],0,NULL,NULL);  
 if ( temp == NULL ) {
   fprintf(stdout,"Failed to open [%s]\n",argv[1]);
   return 1 ; 
 }



 vp = temp->buffer;

 end = vp +  temp->size; 

 sp = vp; 

 if ( sp->type == ROOT_PK) {
    if ( ( rc = verify_element(trust_root, sp)) != 1 ) {
      return 1 ;
    }
    else {
      if ( key == ROOT_PK ) { 
        fprintf(stdout,"next\t0x%x\n",sp->next);
        fprintf(stdout,"type\t%d\n",sp->p.rk_p->type);
        fprintf(stdout,"tag_0\t%d\n",sp->p.rk_p->tag_0);
        fprintf(stdout,"size_0\t%d\n",sp->p.rk_p->size_0);
        fprintf(stdout,"value_0\t0x%x\n",sp->p.rk_p->value_0);
        fprintf(stdout,"tag_1\t%d\n",sp->p.rk_p->tag_1);
        fprintf(stdout,"size_1\t%d\n",sp->p.rk_p->size_1);
        fprintf(stdout,"value_1\t0x%x\n",sp->p.rk_p->value_1);
        if ( 1 ) {
           if ((out = plf_new_file()) != NULL ) {
              out->path="value_0.bin";
              out->mode=0644;
              out->sync=1;
              out->buffer=sp->p.rk_p->value_0; 
              out->size= sp->p.rk_p->size_0; 
              rc = plf_write_file(out);
	      fprintf(stderr,"Written to %s code %d\n",out->path,rc);
              plf_free_file(out);
           }
           if ((out = plf_new_file()) != NULL ) {
              out->path="value_1.bin";
              out->mode=0644;
              out->sync=1;
              out->buffer=sp->p.rk_p->value_1; 
              out->size= sp->p.rk_p->size_1; 
              rc = plf_write_file(out);
	      fprintf(stderr,"Written to %s code %d\n",out->path,rc);
              plf_free_file(out);
           }
        }
      } 
      install_key(&trust_root, sp);
    }
    sp = (struct sobject* ) sp->next;
 }
 else 
  return 1 ;


 len =  end -  sp  ; 
 while (len > 0 ) {
  switch  (sp->type ) {



   case ATF_BOOT : 

  	         rc = verify_element(trust_root, sp); 
  	         if ( rc != SPD_OK ) {
		   return 1 ; 
                 }
                 else {
                   if ( key == ATF_BOOT ) { 
                     fprintf(stdout,"next\t0x%x\n",sp->next);
                     fprintf(stdout,"size\t%d\n",sp->p.bl2_p->size);
		     fprintf(stdout,"start\t0x%x\n",sp->p.bl2_p->start);
		     fprintf(stdout,"bl31_load_address\t0x%x\n",sp->p.bl2_p->bl31_load_address);
		     fprintf(stdout,"bl32_load_address\t0x%x\n",sp->p.bl2_p->bl32_load_address);
		     fprintf(stdout,"bl33_load_address\t0x%x\n",sp->p.bl2_p->bl33_load_address);
		     fprintf(stdout,"e_level\t%d\n",sp->p.bl2_p->e_level);
                   } 
                 }
                 break; 

   case ATF_RUNTIME : 

  	         rc = verify_element(trust_root, sp); 
  	         if ( rc != SPD_OK ) {
		   return 1 ; 
                 }
                 else {
                   if ( key == ATF_RUNTIME ) { 
                     fprintf(stdout,"next\t0x%x\n",sp->next);
                     fprintf(stdout,"size\t%d\n",sp->p.atf_runtime_p->size);
		     fprintf(stdout,"start\t0x%x\n",sp->p.atf_runtime_p->start);
		     fprintf(stdout,"bl31_load_address\t0x%x\n",sp->p.atf_runtime_p->bl31_load_address);
                     if ( 1 ) {
                        if ((out = plf_new_file()) != NULL ) {
                          out->path="bl31.bin";
                          out->mode=0644;
                          out->sync=1;
                          out->buffer=sp->p.atf_runtime_p->start; 
                          out->size= sp->p.atf_runtime_p->size; 
                          rc = plf_write_file(out);
		          fprintf(stderr,"Written to %s code %d\n",out->path,rc);
                        }
                     }
                   } 
                 }
                 break; 


   case ATF_PAYLOAD : 

  	         rc = verify_element(trust_root, sp); 
  	         if ( rc != SPD_OK ) {
		   return 1 ; 
                 }
                 else {
                   if ( key == ATF_PAYLOAD ) { 
                     fprintf(stdout,"next\t0x%x\n",sp->next);
                     fprintf(stdout,"size\t%d\n",sp->p.atf_payload_p->size);
		     fprintf(stdout,"start\t0x%x\n",sp->p.atf_payload_p->start);
		     fprintf(stdout,"bl33_load_address\t0x%x\n",sp->p.atf_payload_p->bl33_load_address);
                     if ( 1 ) {
                        if ((out = plf_new_file()) != NULL ) {
                          out->path="bl33.bin";
                          out->mode=0644;
                          out->sync=1;
                          out->buffer=sp->p.atf_payload_p->start; 
                          out->size= sp->p.atf_payload_p->size; 
                          rc = plf_write_file(out);
		          fprintf(stderr,"Written to %s code %d\n",out->path,rc);
                        }
                     }
                   } 
                 }
                 break; 
   case SDRAM : 

  	         rc = verify_element(trust_root, sp); 
  	         if ( rc != SPD_OK ) {
                   fprintf(stderr,"Failed to verify [rc=%d]\n",rc);
		   return 1 ; 
                 }
                 else {
                   if ( key == SDRAM ) { 
                    fprintf(stdout,"size\t%d\n",sp->p.sdp->size);
                    fprintf(stdout,"image_size\t%d\n",sp->p.sdp->image_size);
                    fprintf(stdout,"start\t0x%x\n",sp->p.sdp->start);
                    fprintf(stdout,"next\t0x%x\n",sp->next);
                    if ( 1 ) {
                        if ((out = plf_new_file()) != NULL ) {
                          out->path="bl30.bin";
                          out->mode=0644;
                          out->sync=1;
                          out->buffer=sp->p.sdp->start; 
                          out->size= sp->p.sdp->image_size; 
                          rc = plf_write_file(out);
		          fprintf(stderr,"Written to %s code %d\n",out->path,rc);
                        }
                    }
                   }
                 }
                 break; 

   case RAMDISK    : 

  	         rc = verify_element(trust_root, sp); 
  	         if ( rc != SPD_OK ) {
                   fprintf(stdout,"Failed to verify [rc=%d]\n",rc);
		   return 1 ; 
                 }
                 else {
                  if ( key == RAMDISK ) { 
                   fprintf(stdout,"size\t%d\n",sp->p.rp->size);
                   fprintf(stdout,"load_address\t0x%x\n",sp->p.rp->load_address);
                   fprintf(stdout,"start\t0x%x\n",sp->p.rp->start);
                   fprintf(stdout,"next\t0x%x\n",sp->next);
		   p = sp->p.rp->start; 
                   b_hdr = sp->p.sdp->start;
		   fprintf(stderr,"%s: [%x] %x %x %x %x\n",__func__, p, p[0], p[1], p[2], p[3]);
		   fprintf(stderr,"magic: 0x%x BUNDLE_MAGIC: 0x%x\n",b_hdr->magic,BUNDLE_MAGIC);
                   if ( 1 ) {
                        if ((out = plf_new_file()) != NULL ) {
                          out->path="ramdisk";
                          out->mode=0644;
                          out->sync=1;
                          out->buffer=sp->p.rp->start; 
                          out->size= sp->p.rp->size; 
                          rc = plf_write_file(out);
		          fprintf(stderr,"Written to %s code %d\n",out->path,rc);
                        }
                   }
                  }
                 }
                 break; 
   case KERNEL    : 

                 fprintf(stdout,"KERNEL\n");
  	         rc = verify_element(trust_root, sp); 
  	         if ( rc != SPD_OK ) {
                   fprintf(stdout,"Failed to verify [rc=%d]\n",rc);
		   return 1 ; 
                 }
                 else {
                  if ( key == KERNEL ) { 
                   fprintf(stdout,"size\t%d\n",sp->p.kernel_p->size);
                   fprintf(stdout,"load_address\t%d\n",sp->p.kernel_p->load_address);
		   fprintf(stdout,"e_level\t%d\n",sp->p.kernel_p->e_level);
                   if ( 1 ) {
                        if ((out = plf_new_file()) != NULL ) {
                          out->path="kernel";
                          out->mode=0644;
                          out->sync=1;
                          out->buffer=sp->p.kernel_p->start; 
                          out->size= sp->p.kernel_p->size; 
                          rc = plf_write_file(out);
		          fprintf(stderr,"Written to %s code %d\n",out->path,rc);
                        }
                   }
                  }
                 }
                 break; 
   case DEVTREE   : 

                 fprintf(stdout,"DEVTREE\n");
  	         rc = verify_element(trust_root, sp); 
  	         if ( rc != SPD_OK ) {
                   fprintf(stdout,"Failed to verify [rc=%d]\n",rc);
		   return 1 ; 
                 }
                 else {
                  if ( key == DEVTREE ) { 
                   fprintf(stdout,"size\t%d\n",sp->p.devtree_p->size);
                   fprintf(stdout,"load_address\t%d\n",sp->p.devtree_p->load_address);
                   if ( 1 ) {
                        if ((out = plf_new_file()) != NULL ) {
                          out->path="devtree";
                          out->mode=0644;
                          out->sync=1;
                          out->buffer=sp->p.devtree_p->start; 
                          out->size= sp->p.devtree_p->size; 
                          rc = plf_write_file(out);
		          fprintf(stderr,"Written to %s code %d\n",out->path,rc);
                   }
                  }
                 }
                 }
                 break; 

   default :     
                 fprintf(stdout,"Unknown type %d\n",sp->type);
		 return 1 ; 
                 break; 
  }
  sp = (struct sobject* ) sp->next; 
  len =  end - sp ; 
 }
 
 plf_free_file(temp);
 return 0; 
} /* main */


