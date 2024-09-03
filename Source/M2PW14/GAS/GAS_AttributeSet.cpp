// 

// Base:
#include "GAS_AttributeSet.h"

// UE:
#include "Net/UnrealNetwork.h"
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

UGAS_AttributeSet::UGAS_AttributeSet()
{
}

void UGAS_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Настройка репликации Атрибутов: Всегда реплицировать
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Sanity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxSanity, COND_None, REPNOTIFY_Always);
}
//--------------------------------------------------------------------------------------



/* ---   AttributeSet   --- */

void UGAS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustingAttributeForMaxValue(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustingAttributeForMaxValue(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxSanityAttribute())
	{
		AdjustingAttributeForMaxValue(Sanity, MaxSanity, NewValue, GetSanityAttribute());
	}
}

void UGAS_AttributeSet::AdjustingAttributeForMaxValue(
	const FGameplayAttributeData& iDependentAttribute,
	const FGameplayAttributeData& iMaxValueAttribute,
	const float iNewMaxValue,
	const FGameplayAttribute& iDependentAttributeProperty)
{
	// Проверка наличия Компонента спсобностей
	if (UAbilitySystemComponent* lAbilityComp = GetOwningAbilitySystemComponent())
	{
		const float lCurrentMaxValue =
			iMaxValueAttribute.GetCurrentValue() == 0
			? iNewMaxValue : iMaxValueAttribute.GetCurrentValue();

		// Проверка на значительное изменение значения
		if (!FMath::IsNearlyEqual(lCurrentMaxValue, iNewMaxValue))
		{
			const float lCurrentValue = iDependentAttribute.GetCurrentValue();
			float lNewCurrentValue = lCurrentValue * iNewMaxValue / lCurrentMaxValue;

			// Изменение значение Свойства на расчитанное
			lAbilityComp->ApplyModToAttributeUnsafe(
				iDependentAttributeProperty,
				EGameplayModOp::Override,
				lNewCurrentValue);
		}
	}
	else
	{
		// LOG: Системный компонент, владеющий способностью, не существует
		UE_LOG(LogClass, Warning, TEXT("GAS_AttributeSet: Owning UAbilitySystemComponent does NOT exist"));
	}
}
//--------------------------------------------------------------------------------------



/* ---   My Attribute   --- */

void UGAS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Health, OldHealth);
}

void UGAS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxHealth, OldMaxHealth);
}

void UGAS_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Mana, OldMana);
}

void UGAS_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxMana, OldMaxMana);
}

void UGAS_AttributeSet::OnRep_Sanity(const FGameplayAttributeData& OldSanity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Sanity, OldSanity);
}

void UGAS_AttributeSet::OnRep_MaxSanity(const FGameplayAttributeData& OldMaxSanity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxSanity, MaxSanity);
}

//--------------------------------------------------------------------------------------
