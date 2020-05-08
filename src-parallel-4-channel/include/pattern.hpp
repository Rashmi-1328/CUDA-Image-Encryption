
#ifndef PATTERN_H
#define PATTERN_H
#include "io.hpp"
namespace pattern 
{
  
  static inline void twodLogisticMapAdvanced(double *&x, double *&y, uint32_t *&random_array, double myu1, double myu2, double lambda1, double   lambda2,uint32_t number);
  static inline void twodLogisticAdjustedSineMap(double *&x, double *&y, uint32_t *&random_array, double myu, uint32_t total);
  static inline void MTSequence(uint32_t *&random_array,uint32_t total,int lower_limit,int upper_limit,int seed);
  static inline void twodSineLogisticModulationMap(double *&x, double *&y,uint32_t *&random_array,double alpha, double beta, uint32_t total);
  static inline void twodLogisticAdjustedLogisticMap(double *&x,double *&y,double *&x_bar,double *&y_bar,uint32_t *&random_array,double myu,uint32_t total);
  static inline void twodLogisticMap(double *&x, double *&y, uint32_t *&random_array,double r,uint32_t total);
  
  static inline void initializeMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,int number_of_rounds);
  
  static inline void assignMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,int number_of_rounds);
  
  static inline void displayMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,int number_of_rounds);  

  static inline long writeMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,FILE *outfile,long ptr_position,int number_of_rounds);

  static inline long readMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,FILE *infile,long ptr_position,int number_of_rounds);

  static inline void selectChaoticMap(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],double* x,double* y,double* x_bar,double* y_bar,uint32_t*& random_array,uint32_t*& lut_vec,config::ChaoticMap map,int iteration,uint32_t m,uint32_t random_array_length);
  

  static inline void twodLogisticMapAdvanced(double *&x, double *&y, uint32_t *&random_array, double myu1, double myu2, double lambda1, double lambda2,uint32_t number)
  {
    //printf("\n In 2dLMA");
    int i = 0;
    for(i = 0; i < number - 1; ++i)
    {
      //printf("\nx= %F",x[i]);
      x[i + 1] = x[i] * myu1 * (1 - x[i]) + lambda1 * (y[i] * y[i]);
      y[i + 1] = y[i] * myu2 * (1 - y[i]) + lambda2 * ((x[i] * x[i]) + x[i] * y[i]); 
    }
     
    for(int i = 0; i < number; ++i)
    {
      random_array[i] = common::get_n_mantissa_bits_safe(x[i],NUMBER_OF_BITS);
    }
  }

  static inline void twodLogisticAdjustedSineMap(double *&x,double *&y,uint32_t *&random_array,double myu,uint32_t total)
  {
    //printf("\nIn 2dLASM");
    int i=0;

    for(i = 0; i < (total) - 1; ++i)
    { 
      //printf("\nx= %F",x[i]);
      //printf("\n%d",random_array[i]);
      x[i + 1] = sin(M_PI * myu * (y[i] + 3) * x[i] * (1 - x[i]));
      y[i + 1] = sin(M_PI * myu * (x[i + 1] + 3) * y[i] * (1 - y[i]));
    }
    
    for(int i = 0; i < total; ++i)
    {
      random_array[i] = common::get_n_mantissa_bits_safe(x[i],NUMBER_OF_BITS);
    }
  }

  static inline void MTSequence(uint32_t *&random_array,uint32_t total,int lower_limit,int upper_limit,int seed)
  {
    //cout<<"\nIn MTMap";
    //std::random_device r;
    //std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 seeder(seed);
    
    std::uniform_int_distribution<int> intGen(lower_limit,upper_limit);
 
    /* generate ten random numbers in [1,6] */
    for (size_t i = 0; i < total; ++i)
    {
        auto random_number = intGen(seeder);
        random_array[i]=(uint32_t)random_number;
    }
  }

  static inline void twodSineLogisticModulationMap(double *&x, double *&y,uint32_t *&random_array,double alpha, double beta, uint32_t total)
  {
    //printf("\nIn 2dSLMM");
    for(int i = 0; i < (total) - 1; ++i)
    {
      x[i + 1] = alpha * (sin(M_PI * y[i]) + beta) * x[i] * (1 - x[i]);
      y[i + 1] = alpha * (sin(M_PI * x[i + 1]) + beta) * y[i] * (1 - y[i]);
    }
    
    for(int i = 0; i < total; ++i)
    {
      random_array[i] = common::get_n_mantissa_bits_safe(x[i],NUMBER_OF_BITS);
    }
  }
  
  static inline void twodLogisticAdjustedLogisticMap(double *&x,double *&y,double *&x_bar,double *&y_bar,uint32_t *&random_array,double myu,uint32_t total)
  {
    //printf("\nIn 2dLALM");
    for(uint32_t i = 0; i < total - 1; ++i)
    {
       x_bar[i + 1] = myu * (y[i] * 3) * x[i] * (1 - x[i]);
       x[i + 1] = 4 * x_bar[i + 1] * (1 - x_bar[i + 1]);
       y_bar[i + 1] = myu * (x[i + 1] + 3) * y[i] * (1 - y[i]);
       y[i + 1] = 4 * y_bar[i + 1] * (1 - y_bar[i + 1]); 
    }
    
    for(int i = 0; i < total; ++i)
    {
      random_array[i] = common::get_n_mantissa_bits_safe(x[i],NUMBER_OF_BITS);
    }
  }

  static inline void twodLogisticMap(double *&x, double *&y, uint32_t *&random_array,double r,uint32_t total)
  {
    //printf("\nIn 2dLM");
    for(uint32_t i = 0; i < (total) - 1; ++i)
    {
      //cout<<"\nindex = "<<i;
      
      x[i + 1] = r * ((3 * y[i]) + 1) * x[i] * (1 - x[i]);
      y[i + 1] = r * ((3 * x[i + 1]) + 1) * y[i] * (1 - y[i]);
    }
    
    for(int i = 0; i < total; ++i)
    {
      random_array[i] = common::get_n_mantissa_bits_safe(x[i],NUMBER_OF_BITS);
    } 
  }
  
  static inline void initializeMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,int number_of_rounds)
  {
    
    //Initializing all parameters to zero
    for(int i = 0; i < number_of_rounds; ++i)
    {
      if(int(map) == 1)
      {
        cout<<"\nInitializing LM parameters for ROUND "<<i;
        lm_parameters[i].x_init = 0.00;
        lm_parameters[i].y_init = 0.00;
        lm_parameters[i].r = 0.00;
      }
      
      if(int(map) == 2)
      {
        cout<<"\nInitializing LMA parameters for ROUND "<<i;
        lma_parameters[i].x_init = 0.00;
        lma_parameters[i].y_init = 0.00;
        lma_parameters[i].myu1 = 0.00;
        lma_parameters[i].myu2 = 0.00;
        lma_parameters[i].lambda1 = 0.00;
        lma_parameters[i].lambda2 = 0.00;
      }
      
      if(int(map) == 3)
      {
        cout<<"\nInitializing SLMM parameters for ROUND "<<i;
        slmm_parameters[i].x_init = 0.00;
        slmm_parameters[i].y_init = 0.00;
        slmm_parameters[i].alpha = 0.00;
        slmm_parameters[i].beta = 0.00;
      }
      
      if(int(map) == 4)
      {
        cout<<"\nInitializing LASM parameters for ROUND "<<i;
        lasm_parameters[i].x_init = 0.00;
        lasm_parameters[i].y_init = 0.00;
        lasm_parameters[i].myu = 0.00;
      } 
      
      if(int(map) == 5)
      {
        cout<<"\nInitializing LALM parameters for ROUND "<<i;
        lalm_parameters[i].x_init = 0.00;
        lalm_parameters[i].y_init = 0.00;
        lalm_parameters[i].myu = 0.00;
      }
      
      if(int(map) == 6)
      {
        cout<<"\nInitializing MT parameters for ROUND "<<i;
        mt_parameters[i].seed_1 = 0;;
      }
    }  
  }
  
  static inline void assignMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,int number_of_rounds)
  {
    //Assigning random values to parameters
    
    for(int i = 0; i < number_of_rounds; ++i)
    {
      if(int(map) == 1)
      {
        cout<<"\nGenerating parameters for ROUND "<<i;
        lm_parameters[i].x_init = common::getRandomDouble(X_LOWER_LIMIT,X_UPPER_LIMIT);
        lm_parameters[i].y_init = common::getRandomDouble(Y_LOWER_LIMIT,Y_UPPER_LIMIT);
        lm_parameters[i].r = common::getRandomDouble(R_LOWER_LIMIT,R_UPPER_LIMIT);
      }
    
      if(int(map) == 2)
      {
        cout<<"\nGenerating LMA parameters for ROUND "<<i;
        lma_parameters[i].x_init = common::getRandomDouble(X_LOWER_LIMIT,X_UPPER_LIMIT);
        lma_parameters[i].y_init = common::getRandomDouble(Y_LOWER_LIMIT,Y_UPPER_LIMIT);
        lma_parameters[i].myu1 =  common::getRandomDouble(MYU1_LOWER_LIMIT,MYU1_UPPER_LIMIT);
        lma_parameters[i].myu2 = common::getRandomDouble(MYU2_LOWER_LIMIT,MYU2_UPPER_LIMIT);
        lma_parameters[i].lambda1 = common::getRandomDouble(LAMBDA1_LOWER_LIMIT,LAMBDA1_UPPER_LIMIT);
        lma_parameters[i].lambda2 = common::getRandomDouble(LAMBDA2_LOWER_LIMIT,LAMBDA2_UPPER_LIMIT);
      }
      
      if(int(map) == 3)
      {
        cout<<"\nGenerating SLMM parameters for ROUND "<<i;
        slmm_parameters[i].x_init = common::getRandomDouble(X_LOWER_LIMIT,X_UPPER_LIMIT);
        slmm_parameters[i].y_init = common::getRandomDouble(Y_LOWER_LIMIT,Y_UPPER_LIMIT);
        slmm_parameters[i].alpha = common::getRandomDouble(ALPHA_LOWER_LIMIT,ALPHA_UPPER_LIMIT);
        slmm_parameters[i].beta = common::getRandomDouble(BETA_LOWER_LIMIT,BETA_UPPER_LIMIT);        
      }
      
      if(int(map) == 4)
      {
        cout<<"\nGenerating LASM parameters for ROUND "<<i;
        lasm_parameters[i].x_init = common::getRandomDouble(X_LOWER_LIMIT,X_UPPER_LIMIT);
        lasm_parameters[i].y_init = common::getRandomDouble(Y_LOWER_LIMIT,Y_UPPER_LIMIT);
        lasm_parameters[i].myu = common::getRandomDouble(MYU_LOWER_LIMIT,MYU_UPPER_LIMIT);
      }
      
      if(int(map) == 5)
      {
        cout<<"\nGenerating LALM parameters for ROUND "<<i;
        lalm_parameters[i].x_init = common::getRandomDouble(X_LOWER_LIMIT,X_UPPER_LIMIT);
        lalm_parameters[i].y_init = common::getRandomDouble(Y_LOWER_LIMIT,Y_UPPER_LIMIT);
        lalm_parameters[i].myu = common::getRandomDouble(MYU_LOWER_LIMIT,MYU_UPPER_LIMIT);
      } 
      
      if(int(map) == 6)
      {
        cout<<"\nGenerating MT parameters for ROUND "<<i;
        mt_parameters[i].seed_1 = common::getRandomInteger(SEED_LOWER_LIMIT,SEED_UPPER_LIMIT);
      }
    }
  }
  
  static inline void displayMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,int number_of_rounds)
  {
    
    for(int i = 0; i < number_of_rounds; ++i)  
    {
      if(int(map) == 1)
      {
        cout<<"\n\nROUND "<<i;
        printf("\nlm_parameters.x_init = %f",lm_parameters[i].x_init);
        printf("\nlm_parameters.y_init = %f",lm_parameters[i].y_init);
        printf("\nlm_parameters.r = %f",lm_parameters[i].r);
      }
      
      if(int(map) == 2)
      {
        cout<<"\n\nROUND "<<i;
        printf("\nlma_parameters.x_init = %f",lma_parameters[i].x_init);
        printf("\nlma_parameters.y_init = %f",lma_parameters[i].y_init);
        printf("\nlma_parameters.myu1 = %f",lma_parameters[i].myu1);
        printf("\nlma_parameters.myu2 = %f",lma_parameters[i].myu2);
        printf("\nlma_parameters.lambda1 = %f",lma_parameters[i].lambda1);
        printf("\nlma_parameters.lambda2 = %f",lma_parameters[i].lambda2);
      }
      
      if(int(map) == 3)
      {
        cout<<"\n\nROUND "<<i;
        printf("\nslmm_parameters.x_init = %f",slmm_parameters[i].x_init);
        printf("\nslmm_parameters.y_init = %f",slmm_parameters[i].y_init);
        printf("\nslmm_parameters.alpha = %f",slmm_parameters[i].alpha);
        printf("\nslmm_parameters.beta = %f",slmm_parameters[i].beta);
      }
      
      if(int(map) == 4)
      {
        cout<<"\n\nROUND "<<i;
        printf("\nlasm parameters.x_init = %f",lasm_parameters[i].x_init);
        printf("\nlasm parameters.y_init = %f",lasm_parameters[i].y_init);
        printf("\nlasm parameters.myu = %f",lasm_parameters[i].myu);
      }  

      if(int(map) == 5)
      {
        cout<<"\n\nROUND "<<i;
        printf("\nlalm parameters.x_init = %f",lalm_parameters[i].x_init);
        printf("\nlalm parameters.y_init = %f",lalm_parameters[i].y_init);
        printf("\nlalm parameters.myu = %f",lalm_parameters[i].myu);
      }
     
      if(int(map) == 6)
      { 
        cout<<"\n\nROUND "<<i;
        printf("\nmt_parameters.seed_1 = %d",mt_parameters[i].seed_1);
      }
    }
 }
  
  static inline long writeMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,FILE *outfile,long ptr_position,int number_of_rounds)
  {
    if(int(map) == 1)
    {
      ptr_position = writeLMParameters(outfile,config::constant_parameters_file_path,"ab",lm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 2)
    {
      ptr_position = writeLMAParameters(outfile,config::constant_parameters_file_path,"ab",lma_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 3)
    {
      ptr_position = writeSLMMParameters(outfile,config::constant_parameters_file_path,"ab",slmm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 4)
    {
      ptr_position = writeLASMParameters(outfile,config::constant_parameters_file_path,"ab",lasm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 5)
    {
      ptr_position = writeLALMParameters(outfile,config::constant_parameters_file_path,"ab",lalm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 6)
    {
      ptr_position = writeMTParameters(outfile,config::constant_parameters_file_path,"ab",mt_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
  }
  
  static inline long readMapParameters(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],config::ChaoticMap map,FILE *infile,long ptr_position,int number_of_rounds)
  {
    if(int(map) == 1)
    {
      ptr_position = readLMParameters(infile,config::constant_parameters_file_path,"rb",lm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 2)
    {
      ptr_position = readLMAParameters(infile,config::constant_parameters_file_path,"rb",lma_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 3)
    {
      ptr_position = readSLMMParameters(infile,config::constant_parameters_file_path,"rb",slmm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 4)
    {
      ptr_position = readLASMParameters(infile,config::constant_parameters_file_path,"rb",lasm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 5)
    {
      ptr_position = readLALMParameters(infile,config::constant_parameters_file_path,"rb",lalm_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
    
    if(int(map) == 6)
    {
      ptr_position = readMTParameters(infile,config::constant_parameters_file_path,"rb",mt_parameters,0,number_of_rounds,ptr_position);
      return ptr_position;
    }
  }
  
  
  static inline void selectChaoticMap(config::lm lm_parameters[],config::lma lma_parameters[],config::slmm slmm_parameters[],config::lasm lasm_parameters[],config::lalm lalm_parameters[],config::mt mt_parameters[],double* x,double* y,double* x_bar,double* y_bar,uint32_t*& random_array,uint32_t*& lut_vec,config::ChaoticMap map,int iteration,uint32_t m,uint32_t random_array_length)
  {
    if(int(map) == 1)
    {
      cout<<"\n\nIteration = "<<iteration;
      cout<<"\nLM chosen";
      
      x[0] = lm_parameters[iteration].x_init;
      y[0] = lm_parameters[iteration].y_init;
      twodLogisticMap(x,y,random_array,lm_parameters[iteration].r,random_array_length);
      common::genLUTVec(lut_vec,m);
      common::swapLUT(lut_vec,random_array,m);
      
    }
    
    else if(int(map) == 2)
    {
      cout<<"\n\nIteration = "<<iteration;
      cout<<"\nLMA chosen";
      x[0] = lma_parameters[iteration].x_init;
      y[0] = lma_parameters[iteration].y_init;
      twodLogisticMapAdvanced(x,y,random_array,lma_parameters[iteration].myu1,lma_parameters[iteration].myu2,lma_parameters[iteration].lambda1,lma_parameters[iteration].lambda2,random_array_length);
      
      common::genLUTVec(lut_vec,m);
      common::swapLUT(lut_vec,random_array,m);
      
    }
    
    else if(int(map) == 3)
    {
      cout<<"\n\nIteration = "<<iteration;
      cout<<"\nSLMM chosen";
      x[0] = slmm_parameters[iteration].x_init;
      y[0] = slmm_parameters[iteration].y_init;
      twodSineLogisticModulationMap(x,y,random_array,slmm_parameters[iteration].alpha,slmm_parameters[iteration].beta,random_array_length);
      
      common::genLUTVec(lut_vec,m);
      common::swapLUT(lut_vec,random_array,m);
      
    }
    
    else if(int(map) == 4)
    {
      cout<<"\n\nIteration = "<<iteration;
      cout<<"\nLASM chosen";
      x[0] = lasm_parameters[iteration].x_init;
      y[0] = lasm_parameters[iteration].y_init;
      twodLogisticAdjustedSineMap(x,y,random_array,lasm_parameters[iteration].myu,random_array_length);
      
      common::genLUTVec(lut_vec,m);
      common::swapLUT(lut_vec,random_array,m);
      
    }
    
    else if(int(map) == 5)
    {
      cout<<"\n\nIteration = "<<iteration;
      cout<<"\nLALM chosen";
      x[0] = lalm_parameters[iteration].x_init;
      y[0] = lalm_parameters[iteration].y_init;
      twodLogisticAdjustedLogisticMap(x,y,x_bar,y_bar,random_array,lalm_parameters[iteration].myu,random_array_length);
      
      common::genLUTVec(lut_vec,m);
      common::swapLUT(lut_vec,random_array,m);
      
    }
    
    else if(int(map) == 6)
    {
      
      cout<<"\n\nIteration = "<<iteration;
      cout<<"\nMT chosen";
      MTSequence(random_array,random_array_length,config::lower_limit,config::upper_limit,mt_parameters[iteration].seed_1);
      
      common::genLUTVec(lut_vec,m);
      common::swapLUT(lut_vec,random_array,m);
      
    }
    
  }
  
}
#endif

