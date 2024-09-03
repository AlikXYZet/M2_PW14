// 

#pragma once

// Base:
#include "CoreMinimal.h"

// GAS:
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Generated:
#include "GAS_AttributeSet.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Attribute   --- */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
//--------------------------------------------------------------------------------------



/**
 *
 */
UCLASS()
class M2PW14_API UGAS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	/* ---   Base   --- */

	UGAS_AttributeSet();

	/**  Возвращает свойства, которые реплицируются в течение всего срока службы канала субъекта */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//-------------------------------------------



	/* ---   AttributeSet   --- */

	/** Вызывается непосредственно перед изменением какого-либо атрибута */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//-------------------------------------------



	/* ---   My Attribute   --- */

	// Показатель Здоровья
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Health);

	// Показатель Максимального количества здоровья
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxHealth);

	// Показатель Маны
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Mana);

	// Показатель Максимального количества маны
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxMana);

	// Показатель Рассудка (здравомыслия)
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Sanity)
	FGameplayAttributeData Sanity;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Sanity);

	// Показатель Максимального количества рассудка
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxSanity)
	FGameplayAttributeData MaxSanity;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxSanity);

	//-------------------------------------------



protected:

	/* ---   AttributeSet   --- */

	/** Метод репликации "Показателя Здоровья"
	@param	iDependentAttribute			- Зависимый Атрибут
	@param	iMaxValueAttribute			- Атрибут Максимального значения
	@param	iNewMaxValue				- Новое максимальное значение
	@param	iDependentAttributeProperty	- Свойство зависимого атрибута
	*/
	void AdjustingAttributeForMaxValue(
		const FGameplayAttributeData& iDependentAttribute,
		const FGameplayAttributeData& iMaxValueAttribute,
		const float iNewMaxValue,
		const FGameplayAttribute& iDependentAttributeProperty);
	//-------------------------------------------



	/* ---   My Attribute   --- */

	/** Метод репликации "Показателя Здоровья" */
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	/** Метод репликации "Показателя Максимального количества здоровья" */
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	/** Метод репликации "Показателя Маны" */
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	/** Метод репликации "Показателя Максимального количества маны" */
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	/** Метод репликации "Показателя Рассудка (здравомыслия)" */
	UFUNCTION()
	virtual void OnRep_Sanity(const FGameplayAttributeData& OldSanity);

	/** Метод репликации "Показателя Максимального количества рассудка" */
	UFUNCTION()
	virtual void OnRep_MaxSanity(const FGameplayAttributeData& OldMaxSanity);
	//-------------------------------------------
};
