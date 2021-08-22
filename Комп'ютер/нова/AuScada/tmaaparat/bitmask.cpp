#include "bitmask.h"
#include "valve_mask.h"
#include <aulib.h>
#include <QStringList>

BitMask::BitMask(AuPlc &plc, QObject *parent) : QObject(parent),
    src(plc)
{

/*
(* Сигнализация состояния дискретных входов в переменной St_i*)
if (K4c_in) then St_i.0:=true; else St_i.0:=false;end_if; (** Пар греющий М/Д*)
if K4f_in then St_i.1:=true; else St_i.1:=false;end_if;
if (K6c_in) then St_i.2:=true; else St_i.2:=false;end_if; (** Вакуум большой М/Д*)
if K6f_in then St_i.3:=true; else St_i.3:=false;end_if;
if (K7c_in) then St_i.4:=true; else St_i.4:=false;end_if; (** Клапан сиропа М/Д*)
if K7f_in then St_i.5:=true; else St_i.5:=false;end_if;
if (K8c_in) then St_i.6:=true; else St_i.6:=false;end_if; (** Клапан БП М/Д*)
if K8f_in then St_i.7:=true; else St_i.7:=false;end_if;
if (K9c_in) then St_i.8:=true; else St_i.8:=false;end_if; (** Клапан ЗП М/Д*)
if K9f_in then St_i.9:=true; else St_i.9:=false;end_if;
if (K10c_in) then St_i.10:=true; else St_i.10:=false;end_if; (** Клапан АммВ М/Д*)
if K10f_in or K10f_in2 then St_i.11:=true; else St_i.11:=false;end_if;
(*if K11c_in then St_i.12:=true; else St_i.12:=false;end_if;*) (*reserv*)
(*if K11f_in then St_i.13:=true; else St_i.13:=false;end_if;*) (*reserv*)
if (K12c_in) then St_i.14:=true; else St_i.14:=false;end_if; (** Пропарка коллектора М/Д*)
if K12f_in then St_i.15:=true; else St_i.15:=false;end_if;
 */
    St_i[QStringLiteral("K4c_in")]=MASK_K4c_in; // 0
    St_i[QStringLiteral("K4f_in")]=MASK_K4f_in; // 1
    St_i[QStringLiteral("K6c_in")]=MASK_K6c_in; // 2
    St_i[QStringLiteral("K6f_in")]=MASK_K6f_in; // 3
    St_i[QStringLiteral("K7c_in")]=MASK_K7c_in; // 4
    St_i[QStringLiteral("K7f_in")]=MASK_K7f_in; // 5
    St_i[QStringLiteral("K8c_in")]=MASK_K8c_in; // 6
    St_i[QStringLiteral("K8f_in")]=MASK_K8f_in; // 7
    St_i[QStringLiteral("K9c_in")]=MASK_K9c_in; // 8
    St_i[QStringLiteral("K9f_in")]=MASK_K9f_in; // 9
    St_i[QStringLiteral("K10c_in")]=MASK_K10c_in; // 10
    St_i[QStringLiteral("K10f_in")]=MASK_K10f_in; // 11
    St_i[QStringLiteral("K11c_in")]=MASK_K11c_in; // 12
    St_i[QStringLiteral("K11f_in")]=MASK_K11f_in; // 13
    St_i[QStringLiteral("K12c_in")]=MASK_K12c_in; // 14
    St_i[QStringLiteral("K12f_in")]=MASK_K12f_in; // 15
/*
(* Сигнализация состояния дискретных входов в переменной St_i1*)
if (K13c_in) then St_i1.0:=true; else St_i1.0:=false;end_if; (** Пропарка верхней части М/Д*)
if K13f_in then St_i1.1:=true; else St_i1.1:=false;end_if;
if (K14c_in) then St_i1.2:=true; else St_i1.2:=false;end_if; (** Пропарка стекол М/Д*)
if K14f_in then St_i1.3:=true; else St_i1.3:=false;end_if;
if (K15c_in) then St_i1.4:=true; else St_i1.4:=false;end_if; (** Пропарка выгрузки М/Д*)
if K15f_in then St_i1.5:=true; else St_i1.5:=false;end_if;
if (K16c_in) then St_i1.6:=true; else St_i1.6:=false;end_if; (** Клапан затравки М/Д*)
if K16f_in then St_i1.7:=true; else St_i1.7:=false;end_if;
if (K17c_in) then St_i1.8:=true; else St_i1.8:=false;end_if; (** Клапан сброса вакуума М/Д*)
if K17f_in then St_i1.9:=true; else St_i1.9:=false;end_if;
if (K18c_in) then St_i1.10:=true; else St_i1.10:=false;end_if; (** Клапан малого вакуума М/Д*)
if K18f_in then St_i1.11:=true; else St_i1.11:=false;end_if;
if (K19c_in) then St_i1.12:=true; else St_i1.12:=false;end_if; (** Клапан выгрузки М/Д*)
if K191f_in then St_i1.13:=true; else St_i1.13:=false;end_if; (** Клапан выгрузки ОТКРЫТО (ПОЛОЖЕНИЕ)*)
if K192f_in then St_i1.14:=true; else St_i1.14:=false;end_if; (** Клапан выгрузки ЗАКРЫТО (ПОЛОЖЕНИЕ)*)
if M1_A_in then St_i1.15:=true; else St_i1.15:=false;end_if; (** Циркулятор ВКЛ/ВЫКЛ (авар. останов.)*)
*/
    St_i1[QStringLiteral("K13c_in")]=MASK_K13c_in; // 0
    St_i1[QStringLiteral("K13f_in")]=MASK_K13f_in; // 1
    St_i1[QStringLiteral("K14c_in")]=MASK_K14c_in; // 2
    St_i1[QStringLiteral("K14f_in")]=MASK_K14f_in; // 3
    St_i1[QStringLiteral("K15c_in")]=MASK_K15c_in; // 4
    St_i1[QStringLiteral("K15f_in")]=MASK_K15f_in; // 5
    St_i1[QStringLiteral("K16c_in")]=MASK_K16c_in; // 6
    St_i1[QStringLiteral("K16f_in")]=MASK_K16f_in; // 7
    St_i1[QStringLiteral("K17c_in")]=MASK_K17c_in; // 8
    St_i1[QStringLiteral("K17f_in")]=MASK_K17f_in; // 9
    St_i1[QStringLiteral("K18c_in")]=MASK_K18c_in; // 10
    St_i1[QStringLiteral("K18f_in")]=MASK_K18f_in; // 11
    St_i1[QStringLiteral("K19c_in")]=MASK_K19c_in; // 12
    St_i1[QStringLiteral("K191f_in")]=MASK_K191f_in; // 13
    St_i1[QStringLiteral("K192f_in")]=MASK_K192f_in; // 14
    St_i1[QStringLiteral("M1_a_in")]=MASK_M1_A_in; // 15

/*
(* Сигнализация состояния дискретных входов в переменной St_i2*)
if Ps22a_in then St_i2.0:=true; else St_i2.0:=false;end_if;
if (K23c_in) then St_i2.1:=true; else St_i2.1:=false;end_if; (** Клапан отвода пропарки М/Д*)
if K23f_in then St_i2.2:=true; else St_i2.2:=false;end_if;
if M1_R_in then St_i2.3:=true; else St_i2.3:=false;end_if;
*/
    St_i2[QStringLiteral("Ps22a_in")]=MASK_Ps22a_in; // 0
    St_i2[QStringLiteral("K23c_in")]=MASK_K23c_in; // 1
    St_i2[QStringLiteral("K23f_in")]=MASK_K23f_in; // 2
    St_i2[QStringLiteral("M1_r_in")]=MASK_M1_R_in; // 3


}


bool BitMask::operator[](QString tag)
{
    bool val=false;
    if(St_i.keys().contains(tag))
        val=(src[QStringLiteral("St_i")].value()&St_i[tag])!=0;
    else if(St_i1.keys().contains(tag))
        val=(src[QStringLiteral("St_i1")].value()&St_i1[tag])!=0;
    else if(St_i2.keys().contains(tag))
        val=(src[QStringLiteral("St_i2")].value()&St_i2[tag])!=0;
    else
        val=src[tag];

    return  val;
}

bool BitMask::containts(QString tag)
{
    bool res=false;
    if(St_i.keys().contains(tag))
        res=true;
    else if(St_i1.keys().contains(tag))
        res=true;
    else if(St_i2.keys().contains(tag))
        res=true;
    else
        res=src.tagsList().contains(tag);

    return  res;
}
