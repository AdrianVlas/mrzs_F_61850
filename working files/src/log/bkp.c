 void put_before_info_in_buf_from_queue (DigRegUniqVarsAddreses *ptDRUVAd){

    //register void* pv;
static  int time_before_start_record_dr = 0;

    //Записуємо попередній cтан сигналів перед аварією
    //Вираховуємо кількість змін сигналів

    //Помічаємо скільки часу пройшло з початку запуску запису
    //!@!*(ptDRUVAd->time_from_start_record_dr) = 0;
    //Вираховуємо кількість змін сигналів
    //!@!*(ptDRUVAd->number_changes_into_dr_record) = 0;
    //Помічаємо кількість нових зрізів
   //!@! *(ptDRUVAd->number_items_dr) = 0; 
     asm volatile(
             "bkpt 1"
             );      
    register long i;
    register QueueChangeMarker *phld  = &hldQueueChangeMarker;
    char chAmontRecords = phld->capacity;//
    

    
    unsigned long *pCmd;long j; 

    *(ptDRUVAd->number_changes_into_dr_record) = 0;
    *(ptDRUVAd->number_items_dr) = 0; 

    //?= i - 1;//point prev 
    //?    if (j < 0)
    //?        j += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    do{
        j = deQueue();
        pCmd =  (unsigned long *)&arRawCmd[j];
        long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
        if(j <=  shIndexWRRawCmd)
            time_before_start_record_dr = j;
        else
            time_before_start_record_dr = j - AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
        time_before_start_record_dr -= shIndexWRRawCmd;
        unsigned int number_changes_into_current_item = arAmontChangesinSlice[j];;
        //.if(*(ptDRUVAd->number_items_dr) != 0)
        //.    time_before_start_record_dr *= (-1);//1;  
        if(*(ptDRUVAd->number_items_dr) != 0)
         *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;//phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem;
          //Записуємо текучий cтан сигналів
          //Мітка часу
        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr +  0] =  time_before_start_record_dr        & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  1] = (time_before_start_record_dr >> 8 ) & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  2] = (time_before_start_record_dr >> 16) & 0xff;//(3-0)
        }else{
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr );
        }
          //Текучий стан сигналів

        memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&pCmd[0],(NUMBER_BYTES_SAMPLE_DR));//.(36-3)                   
          
          //Кількість змін сигналів у порівнянні із попереднім станом
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 36] = number_changes_into_current_item & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 37] = (number_changes_into_current_item >> 8) & 0xff;
        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = number_changes_into_current_item & 0xff;
            //(phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem) & 0xff;
            
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = number_changes_into_current_item & 0xff;
            //((phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem)>> 8) & 0xff;
        }
        else{
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] =  0;
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] =  0;
        }
        //Збільшуємо на один кількість нових зрізів
        *(ptDRUVAd->number_items_dr) += 1;
        if( ++j >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD )//if( ++i >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD ) 
            j -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;   //    i -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;  
    }while( --chAmontRecords > 0  );
    
    //Записуємо попередній cтан сигналів перед аварією
       long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;//.?38

    
    
    unsigned long *pI32 = (unsigned long *)(ptDRUVAd->p_active_functions);
    *(ptDRUVAd->time_from_start_record_dr) = 0;
    
    unsigned int number_changes_into_current_item;  
    _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY( (pCmd), (pI32), N_BIG, number_changes_into_current_item);
    //Записуємо текучий cтан сигналів
     idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
    //      //Мітка часу
    buffer_for_save_dr_record[idx_data_dr +  0] =  *(ptDRUVAd->time_from_start_record_dr)        & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  1] = (*(ptDRUVAd->time_from_start_record_dr) >> 8 ) & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  2] = (*(ptDRUVAd->time_from_start_record_dr) >> 16) & 0xff;//(3-0)

    //      //Текучий стан сигналів
    memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&((pI32)[0]),(NUMBER_BYTES_SAMPLE_DR));//?(36-3)
    
    //      
    //      //Кількість змін сигналів у порівнянні із попереднім станом
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = number_changes_into_current_item & 0xff;
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = (number_changes_into_current_item >> 8) & 0xff;
    //*(ptDRUVAd->number_items_dr) += 1;
    *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;// <<= як бути з доаварійним масивом
     
} 