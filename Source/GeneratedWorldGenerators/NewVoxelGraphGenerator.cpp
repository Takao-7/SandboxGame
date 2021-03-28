// Copyright 2020 Phyronnaz

#include "NewVoxelGraphGenerator.h"

PRAGMA_GENERATED_VOXEL_GRAPH_START

using FVoxelGraphSeed = int32;

#if VOXEL_GRAPH_GENERATED_VERSION == 1
class FNewVoxelGraphGeneratorInstance : public TVoxelGraphGeneratorInstanceHelper<FNewVoxelGraphGeneratorInstance, UNewVoxelGraphGenerator>
{
public:
	struct FParams
	{
		const FName Default_Material;
		const float Height;
		const FVoxelRichCurve Islands;
		const FVoxelRichCurve Land;
		const float Mountain_erosion_strength;
		const FVoxelRichCurve Mountains;
		const FVoxelRichCurve Ocean;
		const FName Snow;
		const float Snow_Height;
		const float Valleys_Height;
	};
	
	class FLocalComputeStruct_LocalValue
	{
	public:
		struct FOutputs
		{
			FOutputs() {}
			
			void Init(const FVoxelGraphOutputsInit& Init)
			{
			}
			
			template<typename T, uint32 Index>
			T Get() const;
			template<typename T, uint32 Index>
			void Set(T Value);
			
			v_flt Value;
		};
		struct FBufferConstant
		{
			FBufferConstant() {}
			
			v_flt Variable_24; // Height = 500.0 output 0
			v_flt Variable_29; // Valleys Height = -0.5 output 0
			v_flt Variable_18; // Mountain erosion strength = 0.04 output 0
		};
		
		struct FBufferX
		{
			FBufferX() {}
			
			v_flt Variable_21; // X output 0
			v_flt Variable_19; // X output 0
			v_flt Variable_33; // X output 0
			v_flt Variable_25; // X output 0
		};
		
		struct FBufferXY
		{
			FBufferXY() {}
			
			v_flt Variable_0; // 2D Noise SDF.+ output 0
		};
		
		FLocalComputeStruct_LocalValue(const FParams& InParams)
			: Params(InParams)
		{
		}
		
		void Init(const FVoxelGeneratorInit& InitStruct)
		{
			////////////////////////////////////////////////////
			//////////////////// Init nodes ////////////////////
			////////////////////////////////////////////////////
			{
				////////////////////////////////////////////////////
				/////////////// Constant nodes init ////////////////
				////////////////////////////////////////////////////
				{
					/////////////////////////////////////////////////////////////////////////////////
					//////// First compute all seeds in case they are used by constant nodes ////////
					/////////////////////////////////////////////////////////////////////////////////
					
					
					////////////////////////////////////////////////////
					///////////// Then init constant nodes /////////////
					////////////////////////////////////////////////////
					
				}
				
				////////////////////////////////////////////////////
				//////////////////// Other inits ///////////////////
				////////////////////////////////////////////////////
				Function0_XYZWithoutCache_Init(InitStruct);
			}
			
			////////////////////////////////////////////////////
			//////////////// Compute constants /////////////////
			////////////////////////////////////////////////////
			{
				// Height = 500.0
				BufferConstant.Variable_24 = Params.Height;
				
				// Valleys Height = -0.5
				BufferConstant.Variable_29 = Params.Valleys_Height;
				
				// Mountain erosion strength = 0.04
				BufferConstant.Variable_18 = Params.Mountain_erosion_strength;
				
			}
		}
		void ComputeX(const FVoxelContext& Context, FBufferX& BufferX) const
		{
			Function0_X_Compute(Context, BufferX);
		}
		void ComputeXYWithCache(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			Function0_XYWithCache_Compute(Context, BufferX, BufferXY);
		}
		void ComputeXYWithoutCache(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			Function0_XYWithoutCache_Compute(Context, BufferX, BufferXY);
		}
		void ComputeXYZWithCache(const FVoxelContext& Context, const FBufferX& BufferX, const FBufferXY& BufferXY, FOutputs& Outputs) const
		{
			Function0_XYZWithCache_Compute(Context, BufferX, BufferXY, Outputs);
		}
		void ComputeXYZWithoutCache(const FVoxelContext& Context, FOutputs& Outputs) const
		{
			Function0_XYZWithoutCache_Compute(Context, Outputs);
		}
		
		inline FBufferX GetBufferX() const { return {}; }
		inline FBufferXY GetBufferXY() const { return {}; }
		inline FOutputs GetOutputs() const { return {}; }
		
	private:
		FBufferConstant BufferConstant;
		
		const FParams& Params;
		
		FVoxelFastNoise _2D_Cubic_Noise_Fractal_0_Noise;
		TStaticArray<uint8, 32> _2D_Cubic_Noise_Fractal_0_LODToOctaves;
		FVoxelFastNoise _2D_Perlin_Noise_Fractal_0_Noise;
		TStaticArray<uint8, 32> _2D_Perlin_Noise_Fractal_0_LODToOctaves;
		FVoxelFastNoise _2D_IQ_Noise_0_Noise;
		TStaticArray<uint8, 32> _2D_IQ_Noise_0_LODToOctaves;
		FVoxelFastNoise _2D_Erosion_0_Noise;
		TStaticArray<uint8, 32> _2D_Erosion_0_LODToOctaves;
		
		///////////////////////////////////////////////////////////////////////
		//////////////////////////// Init functions ///////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_XYZWithoutCache_Init(const FVoxelGeneratorInit& InitStruct)
		{
			// Init of 2D Cubic Noise Fractal
			_2D_Cubic_Noise_Fractal_0_Noise.SetSeed(FVoxelGraphSeed(1337));
			_2D_Cubic_Noise_Fractal_0_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
			_2D_Cubic_Noise_Fractal_0_Noise.SetFractalOctavesAndGain(3, 0.5);
			_2D_Cubic_Noise_Fractal_0_Noise.SetFractalLacunarity(2.0);
			_2D_Cubic_Noise_Fractal_0_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[0] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[1] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[2] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[3] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[4] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[5] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[6] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[7] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[8] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[9] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[10] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[11] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[12] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[13] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[14] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[15] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[16] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[17] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[18] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[19] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[20] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[21] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[22] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[23] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[24] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[25] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[26] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[27] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[28] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[29] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[30] = 3;
			_2D_Cubic_Noise_Fractal_0_LODToOctaves[31] = 3;
			
			// Init of 2D Perlin Noise Fractal
			_2D_Perlin_Noise_Fractal_0_Noise.SetSeed(FVoxelGraphSeed(1337));
			_2D_Perlin_Noise_Fractal_0_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
			_2D_Perlin_Noise_Fractal_0_Noise.SetFractalOctavesAndGain(7, 0.5);
			_2D_Perlin_Noise_Fractal_0_Noise.SetFractalLacunarity(2.0);
			_2D_Perlin_Noise_Fractal_0_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[0] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[1] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[2] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[3] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[4] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[5] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[6] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[7] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[8] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[9] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[10] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[11] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[12] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[13] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[14] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[15] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[16] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[17] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[18] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[19] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[20] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[21] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[22] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[23] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[24] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[25] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[26] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[27] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[28] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[29] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[30] = 7;
			_2D_Perlin_Noise_Fractal_0_LODToOctaves[31] = 7;
			
			// Init of 2D IQ Noise
			_2D_IQ_Noise_0_Noise.SetSeed(FVoxelGraphSeed(1337));
			_2D_IQ_Noise_0_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
			_2D_IQ_Noise_0_Noise.SetFractalOctavesAndGain(15, 0.5);
			_2D_IQ_Noise_0_Noise.SetFractalLacunarity(2.0);
			_2D_IQ_Noise_0_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
			_2D_IQ_Noise_0_Noise.SetMatrixFromRotation_2D(40.0);
			_2D_IQ_Noise_0_LODToOctaves[0] = 15;
			_2D_IQ_Noise_0_LODToOctaves[1] = 15;
			_2D_IQ_Noise_0_LODToOctaves[2] = 15;
			_2D_IQ_Noise_0_LODToOctaves[3] = 15;
			_2D_IQ_Noise_0_LODToOctaves[4] = 15;
			_2D_IQ_Noise_0_LODToOctaves[5] = 15;
			_2D_IQ_Noise_0_LODToOctaves[6] = 15;
			_2D_IQ_Noise_0_LODToOctaves[7] = 15;
			_2D_IQ_Noise_0_LODToOctaves[8] = 15;
			_2D_IQ_Noise_0_LODToOctaves[9] = 15;
			_2D_IQ_Noise_0_LODToOctaves[10] = 15;
			_2D_IQ_Noise_0_LODToOctaves[11] = 15;
			_2D_IQ_Noise_0_LODToOctaves[12] = 15;
			_2D_IQ_Noise_0_LODToOctaves[13] = 15;
			_2D_IQ_Noise_0_LODToOctaves[14] = 15;
			_2D_IQ_Noise_0_LODToOctaves[15] = 15;
			_2D_IQ_Noise_0_LODToOctaves[16] = 15;
			_2D_IQ_Noise_0_LODToOctaves[17] = 15;
			_2D_IQ_Noise_0_LODToOctaves[18] = 15;
			_2D_IQ_Noise_0_LODToOctaves[19] = 15;
			_2D_IQ_Noise_0_LODToOctaves[20] = 15;
			_2D_IQ_Noise_0_LODToOctaves[21] = 15;
			_2D_IQ_Noise_0_LODToOctaves[22] = 15;
			_2D_IQ_Noise_0_LODToOctaves[23] = 15;
			_2D_IQ_Noise_0_LODToOctaves[24] = 15;
			_2D_IQ_Noise_0_LODToOctaves[25] = 15;
			_2D_IQ_Noise_0_LODToOctaves[26] = 15;
			_2D_IQ_Noise_0_LODToOctaves[27] = 15;
			_2D_IQ_Noise_0_LODToOctaves[28] = 15;
			_2D_IQ_Noise_0_LODToOctaves[29] = 15;
			_2D_IQ_Noise_0_LODToOctaves[30] = 15;
			_2D_IQ_Noise_0_LODToOctaves[31] = 15;
			
			// Init of 2D Erosion
			_2D_Erosion_0_Noise.SetSeed(FVoxelGraphSeed(1337));
			_2D_Erosion_0_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
			_2D_Erosion_0_Noise.SetFractalOctavesAndGain(5, 0.5);
			_2D_Erosion_0_Noise.SetFractalLacunarity(2.0);
			_2D_Erosion_0_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
			_2D_Erosion_0_Noise.SetCellularJitter(0.5);
			_2D_Erosion_0_LODToOctaves[0] = 5;
			_2D_Erosion_0_LODToOctaves[1] = 5;
			_2D_Erosion_0_LODToOctaves[2] = 5;
			_2D_Erosion_0_LODToOctaves[3] = 5;
			_2D_Erosion_0_LODToOctaves[4] = 5;
			_2D_Erosion_0_LODToOctaves[5] = 5;
			_2D_Erosion_0_LODToOctaves[6] = 5;
			_2D_Erosion_0_LODToOctaves[7] = 5;
			_2D_Erosion_0_LODToOctaves[8] = 5;
			_2D_Erosion_0_LODToOctaves[9] = 5;
			_2D_Erosion_0_LODToOctaves[10] = 5;
			_2D_Erosion_0_LODToOctaves[11] = 5;
			_2D_Erosion_0_LODToOctaves[12] = 5;
			_2D_Erosion_0_LODToOctaves[13] = 5;
			_2D_Erosion_0_LODToOctaves[14] = 5;
			_2D_Erosion_0_LODToOctaves[15] = 5;
			_2D_Erosion_0_LODToOctaves[16] = 5;
			_2D_Erosion_0_LODToOctaves[17] = 5;
			_2D_Erosion_0_LODToOctaves[18] = 5;
			_2D_Erosion_0_LODToOctaves[19] = 5;
			_2D_Erosion_0_LODToOctaves[20] = 5;
			_2D_Erosion_0_LODToOctaves[21] = 5;
			_2D_Erosion_0_LODToOctaves[22] = 5;
			_2D_Erosion_0_LODToOctaves[23] = 5;
			_2D_Erosion_0_LODToOctaves[24] = 5;
			_2D_Erosion_0_LODToOctaves[25] = 5;
			_2D_Erosion_0_LODToOctaves[26] = 5;
			_2D_Erosion_0_LODToOctaves[27] = 5;
			_2D_Erosion_0_LODToOctaves[28] = 5;
			_2D_Erosion_0_LODToOctaves[29] = 5;
			_2D_Erosion_0_LODToOctaves[30] = 5;
			_2D_Erosion_0_LODToOctaves[31] = 5;
			
		}
		
		///////////////////////////////////////////////////////////////////////
		////////////////////////// Compute functions //////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_X_Compute(const FVoxelContext& Context, FBufferX& BufferX) const
		{
			// X
			BufferX.Variable_21 = Context.GetLocalX();
			
			// X
			BufferX.Variable_19 = Context.GetLocalX();
			
			// X
			BufferX.Variable_33 = Context.GetLocalX();
			
			// X
			BufferX.Variable_25 = Context.GetLocalX();
			
		}
		
		void Function0_XYWithCache_Compute(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			// Y
			v_flt Variable_34; // Y output 0
			Variable_34 = Context.GetLocalY();
			
			// Y
			v_flt Variable_22; // Y output 0
			Variable_22 = Context.GetLocalY();
			
			// Y
			v_flt Variable_26; // Y output 0
			Variable_26 = Context.GetLocalY();
			
			// Y
			v_flt Variable_20; // Y output 0
			Variable_20 = Context.GetLocalY();
			
			// 2D Cubic Noise Fractal
			v_flt Variable_5; // 2D Cubic Noise Fractal output 0
			Variable_5 = _2D_Cubic_Noise_Fractal_0_Noise.GetCubicFractal_2D(BufferX.Variable_25, Variable_26, v_flt(0.0001f), _2D_Cubic_Noise_Fractal_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)]);
			Variable_5 = FMath::Clamp<v_flt>(Variable_5, -0.603205, 0.561365);
			
			// 2D Perlin Noise Fractal
			v_flt Variable_14; // 2D Perlin Noise Fractal output 0
			v_flt Variable_15; // 2D Perlin Noise Fractal output 1
			v_flt Variable_16; // 2D Perlin Noise Fractal output 2
			Variable_14 = _2D_Perlin_Noise_Fractal_0_Noise.GetPerlinFractal_2D_Deriv(BufferX.Variable_19, Variable_20, v_flt(0.001f), _2D_Perlin_Noise_Fractal_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)],Variable_15,Variable_16);
			Variable_14 = FMath::Clamp<v_flt>(Variable_14, -0.669395, 0.641631);
			Variable_15 = FMath::Clamp<v_flt>(Variable_15, -1.804641, 1.967932);
			Variable_16 = FMath::Clamp<v_flt>(Variable_16, -1.915765, 1.797332);
			
			// 2D IQ Noise
			v_flt Variable_32; // 2D IQ Noise output 0
			v_flt _2D_IQ_Noise_0_Temp_1; // 2D IQ Noise output 1
			v_flt _2D_IQ_Noise_0_Temp_2; // 2D IQ Noise output 2
			Variable_32 = _2D_IQ_Noise_0_Noise.IQNoise_2D_Deriv(BufferX.Variable_33, Variable_34, v_flt(0.001f), _2D_IQ_Noise_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)],_2D_IQ_Noise_0_Temp_1,_2D_IQ_Noise_0_Temp_2);
			Variable_32 = FMath::Clamp<v_flt>(Variable_32, -0.900599, 0.924530);
			_2D_IQ_Noise_0_Temp_1 = FMath::Clamp<v_flt>(_2D_IQ_Noise_0_Temp_1, -2.106853, 2.134363);
			_2D_IQ_Noise_0_Temp_2 = FMath::Clamp<v_flt>(_2D_IQ_Noise_0_Temp_2, -2.067427, 2.168546);
			
			// Float Curve: Islands
			v_flt Variable_2; // Float Curve: Islands output 0
			Variable_2 = FVoxelNodeFunctions::GetCurveValue(Params.Islands, Variable_14);
			
			// Float Curve: Land
			v_flt Variable_3; // Float Curve: Land output 0
			Variable_3 = FVoxelNodeFunctions::GetCurveValue(Params.Land, Variable_32);
			
			// *
			v_flt Variable_31; // * output 0
			Variable_31 = Variable_5 * v_flt(100.0f);
			
			// Smooth Step
			v_flt Variable_28; // Smooth Step output 0
			Variable_28 = FVoxelMathNodeFunctions::SmoothStep(BufferConstant.Variable_29, v_flt(0.0f), Variable_14);
			
			// Float Curve: Ocean
			v_flt Variable_1; // Float Curve: Ocean output 0
			Variable_1 = FVoxelNodeFunctions::GetCurveValue(Params.Ocean, Variable_14);
			
			// 2D Erosion
			v_flt Variable_27; // 2D Erosion output 0
			v_flt _2D_Erosion_0_Temp_1; // 2D Erosion output 1
			v_flt _2D_Erosion_0_Temp_2; // 2D Erosion output 2
			Variable_27 = _2D_Erosion_0_Noise.GetErosion_2D(BufferX.Variable_21, Variable_22, v_flt(0.02f), _2D_Erosion_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)], Variable_15, Variable_16, _2D_Erosion_0_Temp_1, _2D_Erosion_0_Temp_2);
			Variable_27 = FMath::Clamp<v_flt>(Variable_27, -1.200000, 1.200000);
			_2D_Erosion_0_Temp_1 = FMath::Clamp<v_flt>(_2D_Erosion_0_Temp_1, -1.200000, 1.200000);
			_2D_Erosion_0_Temp_2 = FMath::Clamp<v_flt>(_2D_Erosion_0_Temp_2, -1.200000, 1.200000);
			
			// Height Splitter
			v_flt Variable_6; // Height Splitter output 0
			v_flt Variable_7; // Height Splitter output 1
			v_flt Variable_8; // Height Splitter output 2
			v_flt Variable_9; // Height Splitter output 3
			{
				TVoxelStaticArray<v_flt, 6> InputsArray;
				TVoxelStaticArray<v_flt, 4> OutputsArray;
				InputsArray[0] = v_flt(-10.0f);
				InputsArray[1] = v_flt(5.0f);
				InputsArray[2] = v_flt(10.0f);
				InputsArray[3] = v_flt(10.0f);
				InputsArray[4] = v_flt(30.0f);
				InputsArray[5] = v_flt(5.0f);
				FVoxelMathNodeFunctions::HeightSplit(Variable_31, InputsArray, OutputsArray);
				Variable_6 = OutputsArray[0];
				Variable_7 = OutputsArray[1];
				Variable_8 = OutputsArray[2];
				Variable_9 = OutputsArray[3];
			}
			
			// *
			v_flt Variable_30; // * output 0
			Variable_30 = Variable_27 * Variable_28 * BufferConstant.Variable_18;
			
			// +
			v_flt Variable_17; // + output 0
			Variable_17 = Variable_14 + Variable_30;
			
			// Lerp
			v_flt Variable_10; // Lerp output 0
			Variable_10 = FVoxelNodeFunctions::Lerp(v_flt(0.0f), Variable_1, Variable_6);
			
			// Lerp
			v_flt Variable_11; // Lerp output 0
			Variable_11 = FVoxelNodeFunctions::Lerp(Variable_10, Variable_2, Variable_7);
			
			// Float Curve: Mountains
			v_flt Variable_4; // Float Curve: Mountains output 0
			Variable_4 = FVoxelNodeFunctions::GetCurveValue(Params.Mountains, Variable_17);
			
			// Lerp
			v_flt Variable_12; // Lerp output 0
			Variable_12 = FVoxelNodeFunctions::Lerp(Variable_11, Variable_3, Variable_8);
			
			// Lerp
			v_flt Variable_13; // Lerp output 0
			Variable_13 = FVoxelNodeFunctions::Lerp(Variable_12, Variable_4, Variable_9);
			
			// 2D Noise SDF.*
			v_flt Variable_37; // 2D Noise SDF.* output 0
			Variable_37 = Variable_13 * BufferConstant.Variable_24;
			
			// 2D Noise SDF.+
			BufferXY.Variable_0 = Variable_37 + v_flt(0.0f);
			
		}
		
		void Function0_XYWithoutCache_Compute(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			// Y
			v_flt Variable_34; // Y output 0
			Variable_34 = Context.GetLocalY();
			
			// Y
			v_flt Variable_22; // Y output 0
			Variable_22 = Context.GetLocalY();
			
			// X
			BufferX.Variable_21 = Context.GetLocalX();
			
			// X
			BufferX.Variable_19 = Context.GetLocalX();
			
			// X
			BufferX.Variable_33 = Context.GetLocalX();
			
			// Y
			v_flt Variable_26; // Y output 0
			Variable_26 = Context.GetLocalY();
			
			// X
			BufferX.Variable_25 = Context.GetLocalX();
			
			// Y
			v_flt Variable_20; // Y output 0
			Variable_20 = Context.GetLocalY();
			
			// 2D Cubic Noise Fractal
			v_flt Variable_5; // 2D Cubic Noise Fractal output 0
			Variable_5 = _2D_Cubic_Noise_Fractal_0_Noise.GetCubicFractal_2D(BufferX.Variable_25, Variable_26, v_flt(0.0001f), _2D_Cubic_Noise_Fractal_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)]);
			Variable_5 = FMath::Clamp<v_flt>(Variable_5, -0.603205, 0.561365);
			
			// 2D Perlin Noise Fractal
			v_flt Variable_14; // 2D Perlin Noise Fractal output 0
			v_flt Variable_15; // 2D Perlin Noise Fractal output 1
			v_flt Variable_16; // 2D Perlin Noise Fractal output 2
			Variable_14 = _2D_Perlin_Noise_Fractal_0_Noise.GetPerlinFractal_2D_Deriv(BufferX.Variable_19, Variable_20, v_flt(0.001f), _2D_Perlin_Noise_Fractal_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)],Variable_15,Variable_16);
			Variable_14 = FMath::Clamp<v_flt>(Variable_14, -0.669395, 0.641631);
			Variable_15 = FMath::Clamp<v_flt>(Variable_15, -1.804641, 1.967932);
			Variable_16 = FMath::Clamp<v_flt>(Variable_16, -1.915765, 1.797332);
			
			// 2D IQ Noise
			v_flt Variable_32; // 2D IQ Noise output 0
			v_flt _2D_IQ_Noise_0_Temp_1; // 2D IQ Noise output 1
			v_flt _2D_IQ_Noise_0_Temp_2; // 2D IQ Noise output 2
			Variable_32 = _2D_IQ_Noise_0_Noise.IQNoise_2D_Deriv(BufferX.Variable_33, Variable_34, v_flt(0.001f), _2D_IQ_Noise_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)],_2D_IQ_Noise_0_Temp_1,_2D_IQ_Noise_0_Temp_2);
			Variable_32 = FMath::Clamp<v_flt>(Variable_32, -0.900599, 0.924530);
			_2D_IQ_Noise_0_Temp_1 = FMath::Clamp<v_flt>(_2D_IQ_Noise_0_Temp_1, -2.106853, 2.134363);
			_2D_IQ_Noise_0_Temp_2 = FMath::Clamp<v_flt>(_2D_IQ_Noise_0_Temp_2, -2.067427, 2.168546);
			
			// Float Curve: Islands
			v_flt Variable_2; // Float Curve: Islands output 0
			Variable_2 = FVoxelNodeFunctions::GetCurveValue(Params.Islands, Variable_14);
			
			// Float Curve: Land
			v_flt Variable_3; // Float Curve: Land output 0
			Variable_3 = FVoxelNodeFunctions::GetCurveValue(Params.Land, Variable_32);
			
			// *
			v_flt Variable_31; // * output 0
			Variable_31 = Variable_5 * v_flt(100.0f);
			
			// Smooth Step
			v_flt Variable_28; // Smooth Step output 0
			Variable_28 = FVoxelMathNodeFunctions::SmoothStep(BufferConstant.Variable_29, v_flt(0.0f), Variable_14);
			
			// Float Curve: Ocean
			v_flt Variable_1; // Float Curve: Ocean output 0
			Variable_1 = FVoxelNodeFunctions::GetCurveValue(Params.Ocean, Variable_14);
			
			// 2D Erosion
			v_flt Variable_27; // 2D Erosion output 0
			v_flt _2D_Erosion_0_Temp_1; // 2D Erosion output 1
			v_flt _2D_Erosion_0_Temp_2; // 2D Erosion output 2
			Variable_27 = _2D_Erosion_0_Noise.GetErosion_2D(BufferX.Variable_21, Variable_22, v_flt(0.02f), _2D_Erosion_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)], Variable_15, Variable_16, _2D_Erosion_0_Temp_1, _2D_Erosion_0_Temp_2);
			Variable_27 = FMath::Clamp<v_flt>(Variable_27, -1.200000, 1.200000);
			_2D_Erosion_0_Temp_1 = FMath::Clamp<v_flt>(_2D_Erosion_0_Temp_1, -1.200000, 1.200000);
			_2D_Erosion_0_Temp_2 = FMath::Clamp<v_flt>(_2D_Erosion_0_Temp_2, -1.200000, 1.200000);
			
			// Height Splitter
			v_flt Variable_6; // Height Splitter output 0
			v_flt Variable_7; // Height Splitter output 1
			v_flt Variable_8; // Height Splitter output 2
			v_flt Variable_9; // Height Splitter output 3
			{
				TVoxelStaticArray<v_flt, 6> InputsArray;
				TVoxelStaticArray<v_flt, 4> OutputsArray;
				InputsArray[0] = v_flt(-10.0f);
				InputsArray[1] = v_flt(5.0f);
				InputsArray[2] = v_flt(10.0f);
				InputsArray[3] = v_flt(10.0f);
				InputsArray[4] = v_flt(30.0f);
				InputsArray[5] = v_flt(5.0f);
				FVoxelMathNodeFunctions::HeightSplit(Variable_31, InputsArray, OutputsArray);
				Variable_6 = OutputsArray[0];
				Variable_7 = OutputsArray[1];
				Variable_8 = OutputsArray[2];
				Variable_9 = OutputsArray[3];
			}
			
			// *
			v_flt Variable_30; // * output 0
			Variable_30 = Variable_27 * Variable_28 * BufferConstant.Variable_18;
			
			// +
			v_flt Variable_17; // + output 0
			Variable_17 = Variable_14 + Variable_30;
			
			// Lerp
			v_flt Variable_10; // Lerp output 0
			Variable_10 = FVoxelNodeFunctions::Lerp(v_flt(0.0f), Variable_1, Variable_6);
			
			// Lerp
			v_flt Variable_11; // Lerp output 0
			Variable_11 = FVoxelNodeFunctions::Lerp(Variable_10, Variable_2, Variable_7);
			
			// Float Curve: Mountains
			v_flt Variable_4; // Float Curve: Mountains output 0
			Variable_4 = FVoxelNodeFunctions::GetCurveValue(Params.Mountains, Variable_17);
			
			// Lerp
			v_flt Variable_12; // Lerp output 0
			Variable_12 = FVoxelNodeFunctions::Lerp(Variable_11, Variable_3, Variable_8);
			
			// Lerp
			v_flt Variable_13; // Lerp output 0
			Variable_13 = FVoxelNodeFunctions::Lerp(Variable_12, Variable_4, Variable_9);
			
			// 2D Noise SDF.*
			v_flt Variable_37; // 2D Noise SDF.* output 0
			Variable_37 = Variable_13 * BufferConstant.Variable_24;
			
			// 2D Noise SDF.+
			BufferXY.Variable_0 = Variable_37 + v_flt(0.0f);
			
		}
		
		void Function0_XYZWithCache_Compute(const FVoxelContext& Context, const FBufferX& BufferX, const FBufferXY& BufferXY, FOutputs& Outputs) const
		{
			// Z
			v_flt Variable_23; // Z output 0
			Variable_23 = Context.GetLocalZ();
			
			// 2D Noise SDF.-
			v_flt Variable_36; // 2D Noise SDF.- output 0
			Variable_36 = Variable_23 - BufferXY.Variable_0;
			
			// Set High Quality Value.*
			v_flt Variable_35; // Set High Quality Value.* output 0
			Variable_35 = Variable_36 * v_flt(0.2f);
			
			Outputs.Value = Variable_35;
		}
		
		void Function0_XYZWithoutCache_Compute(const FVoxelContext& Context, FOutputs& Outputs) const
		{
			// Y
			v_flt Variable_34; // Y output 0
			Variable_34 = Context.GetLocalY();
			
			// Y
			v_flt Variable_22; // Y output 0
			Variable_22 = Context.GetLocalY();
			
			// Z
			v_flt Variable_23; // Z output 0
			Variable_23 = Context.GetLocalZ();
			
			// X
			v_flt Variable_21; // X output 0
			Variable_21 = Context.GetLocalX();
			
			// X
			v_flt Variable_19; // X output 0
			Variable_19 = Context.GetLocalX();
			
			// X
			v_flt Variable_33; // X output 0
			Variable_33 = Context.GetLocalX();
			
			// Y
			v_flt Variable_26; // Y output 0
			Variable_26 = Context.GetLocalY();
			
			// X
			v_flt Variable_25; // X output 0
			Variable_25 = Context.GetLocalX();
			
			// Y
			v_flt Variable_20; // Y output 0
			Variable_20 = Context.GetLocalY();
			
			// 2D Cubic Noise Fractal
			v_flt Variable_5; // 2D Cubic Noise Fractal output 0
			Variable_5 = _2D_Cubic_Noise_Fractal_0_Noise.GetCubicFractal_2D(Variable_25, Variable_26, v_flt(0.0001f), _2D_Cubic_Noise_Fractal_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)]);
			Variable_5 = FMath::Clamp<v_flt>(Variable_5, -0.603205, 0.561365);
			
			// 2D Perlin Noise Fractal
			v_flt Variable_14; // 2D Perlin Noise Fractal output 0
			v_flt Variable_15; // 2D Perlin Noise Fractal output 1
			v_flt Variable_16; // 2D Perlin Noise Fractal output 2
			Variable_14 = _2D_Perlin_Noise_Fractal_0_Noise.GetPerlinFractal_2D_Deriv(Variable_19, Variable_20, v_flt(0.001f), _2D_Perlin_Noise_Fractal_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)],Variable_15,Variable_16);
			Variable_14 = FMath::Clamp<v_flt>(Variable_14, -0.669395, 0.641631);
			Variable_15 = FMath::Clamp<v_flt>(Variable_15, -1.804641, 1.967932);
			Variable_16 = FMath::Clamp<v_flt>(Variable_16, -1.915765, 1.797332);
			
			// 2D IQ Noise
			v_flt Variable_32; // 2D IQ Noise output 0
			v_flt _2D_IQ_Noise_0_Temp_1; // 2D IQ Noise output 1
			v_flt _2D_IQ_Noise_0_Temp_2; // 2D IQ Noise output 2
			Variable_32 = _2D_IQ_Noise_0_Noise.IQNoise_2D_Deriv(Variable_33, Variable_34, v_flt(0.001f), _2D_IQ_Noise_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)],_2D_IQ_Noise_0_Temp_1,_2D_IQ_Noise_0_Temp_2);
			Variable_32 = FMath::Clamp<v_flt>(Variable_32, -0.900599, 0.924530);
			_2D_IQ_Noise_0_Temp_1 = FMath::Clamp<v_flt>(_2D_IQ_Noise_0_Temp_1, -2.106853, 2.134363);
			_2D_IQ_Noise_0_Temp_2 = FMath::Clamp<v_flt>(_2D_IQ_Noise_0_Temp_2, -2.067427, 2.168546);
			
			// Float Curve: Islands
			v_flt Variable_2; // Float Curve: Islands output 0
			Variable_2 = FVoxelNodeFunctions::GetCurveValue(Params.Islands, Variable_14);
			
			// Float Curve: Land
			v_flt Variable_3; // Float Curve: Land output 0
			Variable_3 = FVoxelNodeFunctions::GetCurveValue(Params.Land, Variable_32);
			
			// *
			v_flt Variable_31; // * output 0
			Variable_31 = Variable_5 * v_flt(100.0f);
			
			// Smooth Step
			v_flt Variable_28; // Smooth Step output 0
			Variable_28 = FVoxelMathNodeFunctions::SmoothStep(BufferConstant.Variable_29, v_flt(0.0f), Variable_14);
			
			// Float Curve: Ocean
			v_flt Variable_1; // Float Curve: Ocean output 0
			Variable_1 = FVoxelNodeFunctions::GetCurveValue(Params.Ocean, Variable_14);
			
			// 2D Erosion
			v_flt Variable_27; // 2D Erosion output 0
			v_flt _2D_Erosion_0_Temp_1; // 2D Erosion output 1
			v_flt _2D_Erosion_0_Temp_2; // 2D Erosion output 2
			Variable_27 = _2D_Erosion_0_Noise.GetErosion_2D(Variable_21, Variable_22, v_flt(0.02f), _2D_Erosion_0_LODToOctaves[FMath::Clamp(Context.LOD, 0, 31)], Variable_15, Variable_16, _2D_Erosion_0_Temp_1, _2D_Erosion_0_Temp_2);
			Variable_27 = FMath::Clamp<v_flt>(Variable_27, -1.200000, 1.200000);
			_2D_Erosion_0_Temp_1 = FMath::Clamp<v_flt>(_2D_Erosion_0_Temp_1, -1.200000, 1.200000);
			_2D_Erosion_0_Temp_2 = FMath::Clamp<v_flt>(_2D_Erosion_0_Temp_2, -1.200000, 1.200000);
			
			// Height Splitter
			v_flt Variable_6; // Height Splitter output 0
			v_flt Variable_7; // Height Splitter output 1
			v_flt Variable_8; // Height Splitter output 2
			v_flt Variable_9; // Height Splitter output 3
			{
				TVoxelStaticArray<v_flt, 6> InputsArray;
				TVoxelStaticArray<v_flt, 4> OutputsArray;
				InputsArray[0] = v_flt(-10.0f);
				InputsArray[1] = v_flt(5.0f);
				InputsArray[2] = v_flt(10.0f);
				InputsArray[3] = v_flt(10.0f);
				InputsArray[4] = v_flt(30.0f);
				InputsArray[5] = v_flt(5.0f);
				FVoxelMathNodeFunctions::HeightSplit(Variable_31, InputsArray, OutputsArray);
				Variable_6 = OutputsArray[0];
				Variable_7 = OutputsArray[1];
				Variable_8 = OutputsArray[2];
				Variable_9 = OutputsArray[3];
			}
			
			// *
			v_flt Variable_30; // * output 0
			Variable_30 = Variable_27 * Variable_28 * BufferConstant.Variable_18;
			
			// +
			v_flt Variable_17; // + output 0
			Variable_17 = Variable_14 + Variable_30;
			
			// Lerp
			v_flt Variable_10; // Lerp output 0
			Variable_10 = FVoxelNodeFunctions::Lerp(v_flt(0.0f), Variable_1, Variable_6);
			
			// Lerp
			v_flt Variable_11; // Lerp output 0
			Variable_11 = FVoxelNodeFunctions::Lerp(Variable_10, Variable_2, Variable_7);
			
			// Float Curve: Mountains
			v_flt Variable_4; // Float Curve: Mountains output 0
			Variable_4 = FVoxelNodeFunctions::GetCurveValue(Params.Mountains, Variable_17);
			
			// Lerp
			v_flt Variable_12; // Lerp output 0
			Variable_12 = FVoxelNodeFunctions::Lerp(Variable_11, Variable_3, Variable_8);
			
			// Lerp
			v_flt Variable_13; // Lerp output 0
			Variable_13 = FVoxelNodeFunctions::Lerp(Variable_12, Variable_4, Variable_9);
			
			// 2D Noise SDF.*
			v_flt Variable_37; // 2D Noise SDF.* output 0
			Variable_37 = Variable_13 * BufferConstant.Variable_24;
			
			// 2D Noise SDF.+
			v_flt Variable_0; // 2D Noise SDF.+ output 0
			Variable_0 = Variable_37 + v_flt(0.0f);
			
			// 2D Noise SDF.-
			v_flt Variable_36; // 2D Noise SDF.- output 0
			Variable_36 = Variable_23 - Variable_0;
			
			// Set High Quality Value.*
			v_flt Variable_35; // Set High Quality Value.* output 0
			Variable_35 = Variable_36 * v_flt(0.2f);
			
			Outputs.Value = Variable_35;
		}
		
	};
	class FLocalComputeStruct_LocalMaterial
	{
	public:
		struct FOutputs
		{
			FOutputs() {}
			
			void Init(const FVoxelGraphOutputsInit& Init)
			{
				MaterialBuilder.SetMaterialConfig(Init.MaterialConfig);
			}
			
			template<typename T, uint32 Index>
			T Get() const;
			template<typename T, uint32 Index>
			void Set(T Value);
			
			FVoxelMaterialBuilder MaterialBuilder;
		};
		struct FBufferConstant
		{
			FBufferConstant() {}
			
			int32 Variable_1; // Get Material Collection Index: Snow output 0
			v_flt Variable_2; // Snow Height = 400.0 output 0
			int32 Variable_0; // Get Material Collection Index: Default Material output 0
		};
		
		struct FBufferX
		{
			FBufferX() {}
			
		};
		
		struct FBufferXY
		{
			FBufferXY() {}
			
		};
		
		FLocalComputeStruct_LocalMaterial(const FParams& InParams)
			: Params(InParams)
		{
		}
		
		void Init(const FVoxelGeneratorInit& InitStruct)
		{
			////////////////////////////////////////////////////
			//////////////////// Init nodes ////////////////////
			////////////////////////////////////////////////////
			{
				////////////////////////////////////////////////////
				/////////////// Constant nodes init ////////////////
				////////////////////////////////////////////////////
				{
					/////////////////////////////////////////////////////////////////////////////////
					//////// First compute all seeds in case they are used by constant nodes ////////
					/////////////////////////////////////////////////////////////////////////////////
					
					
					////////////////////////////////////////////////////
					///////////// Then init constant nodes /////////////
					////////////////////////////////////////////////////
					
					// Init of Get Material Collection Index: Snow
					if (InitStruct.MaterialCollection)
					{
						Get_Material_Collection_Index__Snow_0_Index = InitStruct.MaterialCollection->GetMaterialIndex(Params.Snow);
					}
					else
					{
						Get_Material_Collection_Index__Snow_0_Index = -1;
					}
					
					// Init of Get Material Collection Index: Default Material
					if (InitStruct.MaterialCollection)
					{
						Get_Material_Collection_Index__Default_Material_0_Index = InitStruct.MaterialCollection->GetMaterialIndex(Params.Default_Material);
					}
					else
					{
						Get_Material_Collection_Index__Default_Material_0_Index = -1;
					}
					
				}
				
				////////////////////////////////////////////////////
				//////////////////// Other inits ///////////////////
				////////////////////////////////////////////////////
				Function0_XYZWithoutCache_Init(InitStruct);
			}
			
			////////////////////////////////////////////////////
			//////////////// Compute constants /////////////////
			////////////////////////////////////////////////////
			{
				// Get Material Collection Index: Snow
				BufferConstant.Variable_1 = Get_Material_Collection_Index__Snow_0_Index;
				
				// Snow Height = 400.0
				BufferConstant.Variable_2 = Params.Snow_Height;
				
				// Get Material Collection Index: Default Material
				BufferConstant.Variable_0 = Get_Material_Collection_Index__Default_Material_0_Index;
				
			}
		}
		void ComputeX(const FVoxelContext& Context, FBufferX& BufferX) const
		{
			Function0_X_Compute(Context, BufferX);
		}
		void ComputeXYWithCache(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			Function0_XYWithCache_Compute(Context, BufferX, BufferXY);
		}
		void ComputeXYWithoutCache(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			Function0_XYWithoutCache_Compute(Context, BufferX, BufferXY);
		}
		void ComputeXYZWithCache(const FVoxelContext& Context, const FBufferX& BufferX, const FBufferXY& BufferXY, FOutputs& Outputs) const
		{
			Function0_XYZWithCache_Compute(Context, BufferX, BufferXY, Outputs);
		}
		void ComputeXYZWithoutCache(const FVoxelContext& Context, FOutputs& Outputs) const
		{
			Function0_XYZWithoutCache_Compute(Context, Outputs);
		}
		
		inline FBufferX GetBufferX() const { return {}; }
		inline FBufferXY GetBufferXY() const { return {}; }
		inline FOutputs GetOutputs() const { return {}; }
		
	private:
		FBufferConstant BufferConstant;
		
		const FParams& Params;
		
		int32 Get_Material_Collection_Index__Snow_0_Index;
		int32 Get_Material_Collection_Index__Default_Material_0_Index;
		
		///////////////////////////////////////////////////////////////////////
		//////////////////////////// Init functions ///////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_XYZWithoutCache_Init(const FVoxelGeneratorInit& InitStruct)
		{
		}
		
		///////////////////////////////////////////////////////////////////////
		////////////////////////// Compute functions //////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_X_Compute(const FVoxelContext& Context, FBufferX& BufferX) const
		{
		}
		
		void Function0_XYWithCache_Compute(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
		}
		
		void Function0_XYWithoutCache_Compute(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
		}
		
		void Function0_XYZWithCache_Compute(const FVoxelContext& Context, const FBufferX& BufferX, const FBufferXY& BufferXY, FOutputs& Outputs) const
		{
			// Global Z
			v_flt Variable_5; // Global Z output 0
			Variable_5 = Context.GetWorldZ();
			
			// Height Splitter
			v_flt Variable_3; // Height Splitter output 0
			v_flt Variable_4; // Height Splitter output 1
			{
				TVoxelStaticArray<v_flt, 2> InputsArray;
				TVoxelStaticArray<v_flt, 2> OutputsArray;
				InputsArray[0] = BufferConstant.Variable_2;
				InputsArray[1] = v_flt(200.0f);
				FVoxelMathNodeFunctions::HeightSplit(Variable_5, InputsArray, OutputsArray);
				Variable_3 = OutputsArray[0];
				Variable_4 = OutputsArray[1];
			}
			
			Outputs.MaterialBuilder.AddMultiIndex(BufferConstant.Variable_0, Variable_3, bool(false));
			Outputs.MaterialBuilder.AddMultiIndex(BufferConstant.Variable_1, Variable_4, bool(true));
		}
		
		void Function0_XYZWithoutCache_Compute(const FVoxelContext& Context, FOutputs& Outputs) const
		{
			// Global Z
			v_flt Variable_5; // Global Z output 0
			Variable_5 = Context.GetWorldZ();
			
			// Height Splitter
			v_flt Variable_3; // Height Splitter output 0
			v_flt Variable_4; // Height Splitter output 1
			{
				TVoxelStaticArray<v_flt, 2> InputsArray;
				TVoxelStaticArray<v_flt, 2> OutputsArray;
				InputsArray[0] = BufferConstant.Variable_2;
				InputsArray[1] = v_flt(200.0f);
				FVoxelMathNodeFunctions::HeightSplit(Variable_5, InputsArray, OutputsArray);
				Variable_3 = OutputsArray[0];
				Variable_4 = OutputsArray[1];
			}
			
			Outputs.MaterialBuilder.AddMultiIndex(BufferConstant.Variable_0, Variable_3, bool(false));
			Outputs.MaterialBuilder.AddMultiIndex(BufferConstant.Variable_1, Variable_4, bool(true));
		}
		
	};
	class FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ
	{
	public:
		struct FOutputs
		{
			FOutputs() {}
			
			void Init(const FVoxelGraphOutputsInit& Init)
			{
			}
			
			template<typename T, uint32 Index>
			T Get() const;
			template<typename T, uint32 Index>
			void Set(T Value);
			
			v_flt UpVectorX;
			v_flt UpVectorY;
			v_flt UpVectorZ;
		};
		struct FBufferConstant
		{
			FBufferConstant() {}
			
		};
		
		struct FBufferX
		{
			FBufferX() {}
			
		};
		
		struct FBufferXY
		{
			FBufferXY() {}
			
		};
		
		FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ(const FParams& InParams)
			: Params(InParams)
		{
		}
		
		void Init(const FVoxelGeneratorInit& InitStruct)
		{
			////////////////////////////////////////////////////
			//////////////////// Init nodes ////////////////////
			////////////////////////////////////////////////////
			{
				////////////////////////////////////////////////////
				/////////////// Constant nodes init ////////////////
				////////////////////////////////////////////////////
				{
					/////////////////////////////////////////////////////////////////////////////////
					//////// First compute all seeds in case they are used by constant nodes ////////
					/////////////////////////////////////////////////////////////////////////////////
					
					
					////////////////////////////////////////////////////
					///////////// Then init constant nodes /////////////
					////////////////////////////////////////////////////
					
				}
				
				////////////////////////////////////////////////////
				//////////////////// Other inits ///////////////////
				////////////////////////////////////////////////////
				Function0_XYZWithoutCache_Init(InitStruct);
			}
			
			////////////////////////////////////////////////////
			//////////////// Compute constants /////////////////
			////////////////////////////////////////////////////
			{
			}
		}
		void ComputeX(const FVoxelContext& Context, FBufferX& BufferX) const
		{
			Function0_X_Compute(Context, BufferX);
		}
		void ComputeXYWithCache(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			Function0_XYWithCache_Compute(Context, BufferX, BufferXY);
		}
		void ComputeXYWithoutCache(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
			Function0_XYWithoutCache_Compute(Context, BufferX, BufferXY);
		}
		void ComputeXYZWithCache(const FVoxelContext& Context, const FBufferX& BufferX, const FBufferXY& BufferXY, FOutputs& Outputs) const
		{
			Function0_XYZWithCache_Compute(Context, BufferX, BufferXY, Outputs);
		}
		void ComputeXYZWithoutCache(const FVoxelContext& Context, FOutputs& Outputs) const
		{
			Function0_XYZWithoutCache_Compute(Context, Outputs);
		}
		
		inline FBufferX GetBufferX() const { return {}; }
		inline FBufferXY GetBufferXY() const { return {}; }
		inline FOutputs GetOutputs() const { return {}; }
		
	private:
		FBufferConstant BufferConstant;
		
		const FParams& Params;
		
		
		///////////////////////////////////////////////////////////////////////
		//////////////////////////// Init functions ///////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_XYZWithoutCache_Init(const FVoxelGeneratorInit& InitStruct)
		{
		}
		
		///////////////////////////////////////////////////////////////////////
		////////////////////////// Compute functions //////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_X_Compute(const FVoxelContext& Context, FBufferX& BufferX) const
		{
		}
		
		void Function0_XYWithCache_Compute(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
		}
		
		void Function0_XYWithoutCache_Compute(const FVoxelContext& Context, FBufferX& BufferX, FBufferXY& BufferXY) const
		{
		}
		
		void Function0_XYZWithCache_Compute(const FVoxelContext& Context, const FBufferX& BufferX, const FBufferXY& BufferXY, FOutputs& Outputs) const
		{
		}
		
		void Function0_XYZWithoutCache_Compute(const FVoxelContext& Context, FOutputs& Outputs) const
		{
		}
		
	};
	class FLocalComputeStruct_LocalValueRangeAnalysis
	{
	public:
		struct FOutputs
		{
			FOutputs() {}
			
			void Init(const FVoxelGraphOutputsInit& Init)
			{
			}
			
			template<typename T, uint32 Index>
			TVoxelRange<T> Get() const;
			template<typename T, uint32 Index>
			void Set(TVoxelRange<T> Value);
			
			TVoxelRange<v_flt> Value;
		};
		struct FBufferConstant
		{
			FBufferConstant() {}
			
			TVoxelRange<v_flt> Variable_0; // 2D Noise SDF.+ output 0
		};
		
		struct FBufferX
		{
			FBufferX() {}
			
		};
		
		struct FBufferXY
		{
			FBufferXY() {}
			
		};
		
		FLocalComputeStruct_LocalValueRangeAnalysis(const FParams& InParams)
			: Params(InParams)
		{
		}
		
		void Init(const FVoxelGeneratorInit& InitStruct)
		{
			////////////////////////////////////////////////////
			//////////////////// Init nodes ////////////////////
			////////////////////////////////////////////////////
			{
				////////////////////////////////////////////////////
				/////////////// Constant nodes init ////////////////
				////////////////////////////////////////////////////
				{
					/////////////////////////////////////////////////////////////////////////////////
					//////// First compute all seeds in case they are used by constant nodes ////////
					/////////////////////////////////////////////////////////////////////////////////
					
					
					////////////////////////////////////////////////////
					///////////// Then init constant nodes /////////////
					////////////////////////////////////////////////////
					
					// Init of 2D Perlin Noise Fractal
					_2D_Perlin_Noise_Fractal_1_Noise.SetSeed(FVoxelGraphSeed(1337));
					_2D_Perlin_Noise_Fractal_1_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
					_2D_Perlin_Noise_Fractal_1_Noise.SetFractalOctavesAndGain(7, 0.5);
					_2D_Perlin_Noise_Fractal_1_Noise.SetFractalLacunarity(2.0);
					_2D_Perlin_Noise_Fractal_1_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[0] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[1] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[2] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[3] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[4] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[5] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[6] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[7] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[8] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[9] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[10] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[11] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[12] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[13] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[14] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[15] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[16] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[17] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[18] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[19] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[20] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[21] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[22] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[23] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[24] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[25] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[26] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[27] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[28] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[29] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[30] = 7;
					_2D_Perlin_Noise_Fractal_1_LODToOctaves[31] = 7;
					
					// Init of 2D Erosion
					_2D_Erosion_1_Noise.SetSeed(FVoxelGraphSeed(1337));
					_2D_Erosion_1_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
					_2D_Erosion_1_Noise.SetFractalOctavesAndGain(5, 0.5);
					_2D_Erosion_1_Noise.SetFractalLacunarity(2.0);
					_2D_Erosion_1_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
					_2D_Erosion_1_Noise.SetCellularJitter(0.5);
					_2D_Erosion_1_LODToOctaves[0] = 5;
					_2D_Erosion_1_LODToOctaves[1] = 5;
					_2D_Erosion_1_LODToOctaves[2] = 5;
					_2D_Erosion_1_LODToOctaves[3] = 5;
					_2D_Erosion_1_LODToOctaves[4] = 5;
					_2D_Erosion_1_LODToOctaves[5] = 5;
					_2D_Erosion_1_LODToOctaves[6] = 5;
					_2D_Erosion_1_LODToOctaves[7] = 5;
					_2D_Erosion_1_LODToOctaves[8] = 5;
					_2D_Erosion_1_LODToOctaves[9] = 5;
					_2D_Erosion_1_LODToOctaves[10] = 5;
					_2D_Erosion_1_LODToOctaves[11] = 5;
					_2D_Erosion_1_LODToOctaves[12] = 5;
					_2D_Erosion_1_LODToOctaves[13] = 5;
					_2D_Erosion_1_LODToOctaves[14] = 5;
					_2D_Erosion_1_LODToOctaves[15] = 5;
					_2D_Erosion_1_LODToOctaves[16] = 5;
					_2D_Erosion_1_LODToOctaves[17] = 5;
					_2D_Erosion_1_LODToOctaves[18] = 5;
					_2D_Erosion_1_LODToOctaves[19] = 5;
					_2D_Erosion_1_LODToOctaves[20] = 5;
					_2D_Erosion_1_LODToOctaves[21] = 5;
					_2D_Erosion_1_LODToOctaves[22] = 5;
					_2D_Erosion_1_LODToOctaves[23] = 5;
					_2D_Erosion_1_LODToOctaves[24] = 5;
					_2D_Erosion_1_LODToOctaves[25] = 5;
					_2D_Erosion_1_LODToOctaves[26] = 5;
					_2D_Erosion_1_LODToOctaves[27] = 5;
					_2D_Erosion_1_LODToOctaves[28] = 5;
					_2D_Erosion_1_LODToOctaves[29] = 5;
					_2D_Erosion_1_LODToOctaves[30] = 5;
					_2D_Erosion_1_LODToOctaves[31] = 5;
					
					// Init of 2D IQ Noise
					_2D_IQ_Noise_1_Noise.SetSeed(FVoxelGraphSeed(1337));
					_2D_IQ_Noise_1_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
					_2D_IQ_Noise_1_Noise.SetFractalOctavesAndGain(15, 0.5);
					_2D_IQ_Noise_1_Noise.SetFractalLacunarity(2.0);
					_2D_IQ_Noise_1_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
					_2D_IQ_Noise_1_Noise.SetMatrixFromRotation_2D(40.0);
					_2D_IQ_Noise_1_LODToOctaves[0] = 15;
					_2D_IQ_Noise_1_LODToOctaves[1] = 15;
					_2D_IQ_Noise_1_LODToOctaves[2] = 15;
					_2D_IQ_Noise_1_LODToOctaves[3] = 15;
					_2D_IQ_Noise_1_LODToOctaves[4] = 15;
					_2D_IQ_Noise_1_LODToOctaves[5] = 15;
					_2D_IQ_Noise_1_LODToOctaves[6] = 15;
					_2D_IQ_Noise_1_LODToOctaves[7] = 15;
					_2D_IQ_Noise_1_LODToOctaves[8] = 15;
					_2D_IQ_Noise_1_LODToOctaves[9] = 15;
					_2D_IQ_Noise_1_LODToOctaves[10] = 15;
					_2D_IQ_Noise_1_LODToOctaves[11] = 15;
					_2D_IQ_Noise_1_LODToOctaves[12] = 15;
					_2D_IQ_Noise_1_LODToOctaves[13] = 15;
					_2D_IQ_Noise_1_LODToOctaves[14] = 15;
					_2D_IQ_Noise_1_LODToOctaves[15] = 15;
					_2D_IQ_Noise_1_LODToOctaves[16] = 15;
					_2D_IQ_Noise_1_LODToOctaves[17] = 15;
					_2D_IQ_Noise_1_LODToOctaves[18] = 15;
					_2D_IQ_Noise_1_LODToOctaves[19] = 15;
					_2D_IQ_Noise_1_LODToOctaves[20] = 15;
					_2D_IQ_Noise_1_LODToOctaves[21] = 15;
					_2D_IQ_Noise_1_LODToOctaves[22] = 15;
					_2D_IQ_Noise_1_LODToOctaves[23] = 15;
					_2D_IQ_Noise_1_LODToOctaves[24] = 15;
					_2D_IQ_Noise_1_LODToOctaves[25] = 15;
					_2D_IQ_Noise_1_LODToOctaves[26] = 15;
					_2D_IQ_Noise_1_LODToOctaves[27] = 15;
					_2D_IQ_Noise_1_LODToOctaves[28] = 15;
					_2D_IQ_Noise_1_LODToOctaves[29] = 15;
					_2D_IQ_Noise_1_LODToOctaves[30] = 15;
					_2D_IQ_Noise_1_LODToOctaves[31] = 15;
					
					// Init of 2D Cubic Noise Fractal
					_2D_Cubic_Noise_Fractal_1_Noise.SetSeed(FVoxelGraphSeed(1337));
					_2D_Cubic_Noise_Fractal_1_Noise.SetInterpolation(EVoxelNoiseInterpolation::Quintic);
					_2D_Cubic_Noise_Fractal_1_Noise.SetFractalOctavesAndGain(3, 0.5);
					_2D_Cubic_Noise_Fractal_1_Noise.SetFractalLacunarity(2.0);
					_2D_Cubic_Noise_Fractal_1_Noise.SetFractalType(EVoxelNoiseFractalType::FBM);
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[0] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[1] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[2] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[3] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[4] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[5] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[6] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[7] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[8] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[9] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[10] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[11] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[12] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[13] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[14] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[15] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[16] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[17] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[18] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[19] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[20] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[21] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[22] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[23] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[24] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[25] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[26] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[27] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[28] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[29] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[30] = 3;
					_2D_Cubic_Noise_Fractal_1_LODToOctaves[31] = 3;
					
				}
				
				////////////////////////////////////////////////////
				//////////////////// Other inits ///////////////////
				////////////////////////////////////////////////////
				Function0_XYZWithoutCache_Init(InitStruct);
			}
			
			////////////////////////////////////////////////////
			//////////////// Compute constants /////////////////
			////////////////////////////////////////////////////
			{
				// 2D Perlin Noise Fractal
				TVoxelRange<v_flt> Variable_14; // 2D Perlin Noise Fractal output 0
				TVoxelRange<v_flt> _2D_Perlin_Noise_Fractal_1_Temp_1; // 2D Perlin Noise Fractal output 1
				TVoxelRange<v_flt> _2D_Perlin_Noise_Fractal_1_Temp_2; // 2D Perlin Noise Fractal output 2
				Variable_14 = { -0.669395f, 0.641631f };
				_2D_Perlin_Noise_Fractal_1_Temp_1 = { -1.804641f, 1.967932f };
				_2D_Perlin_Noise_Fractal_1_Temp_2 = { -1.915765f, 1.797332f };
				
				// Height = 500.0
				TVoxelRange<v_flt> Variable_18; // Height = 500.0 output 0
				Variable_18 = Params.Height;
				
				// 2D Erosion
				TVoxelRange<v_flt> Variable_19; // 2D Erosion output 0
				TVoxelRange<v_flt> _2D_Erosion_1_Temp_1; // 2D Erosion output 1
				TVoxelRange<v_flt> _2D_Erosion_1_Temp_2; // 2D Erosion output 2
				Variable_19 = { -1.200000f, 1.200000f };
				_2D_Erosion_1_Temp_1 = { -1.200000f, 1.200000f };
				_2D_Erosion_1_Temp_2 = { -1.200000f, 1.200000f };
				
				// 2D IQ Noise
				TVoxelRange<v_flt> Variable_24; // 2D IQ Noise output 0
				TVoxelRange<v_flt> _2D_IQ_Noise_1_Temp_1; // 2D IQ Noise output 1
				TVoxelRange<v_flt> _2D_IQ_Noise_1_Temp_2; // 2D IQ Noise output 2
				Variable_24 = { -0.900599f, 0.924530f };
				_2D_IQ_Noise_1_Temp_1 = { -2.106853f, 2.134363f };
				_2D_IQ_Noise_1_Temp_2 = { -2.067427f, 2.168546f };
				
				// 2D Cubic Noise Fractal
				TVoxelRange<v_flt> Variable_5; // 2D Cubic Noise Fractal output 0
				Variable_5 = { -0.603205f, 0.561365f };
				
				// Valleys Height = -0.5
				TVoxelRange<v_flt> Variable_21; // Valleys Height = -0.5 output 0
				Variable_21 = Params.Valleys_Height;
				
				// Mountain erosion strength = 0.04
				TVoxelRange<v_flt> Variable_16; // Mountain erosion strength = 0.04 output 0
				Variable_16 = Params.Mountain_erosion_strength;
				
				// Float Curve: Land
				TVoxelRange<v_flt> Variable_3; // Float Curve: Land output 0
				Variable_3 = FVoxelNodeFunctions::GetCurveValue(Params.Land, Variable_24);
				
				// Float Curve: Islands
				TVoxelRange<v_flt> Variable_2; // Float Curve: Islands output 0
				Variable_2 = FVoxelNodeFunctions::GetCurveValue(Params.Islands, Variable_14);
				
				// Float Curve: Ocean
				TVoxelRange<v_flt> Variable_1; // Float Curve: Ocean output 0
				Variable_1 = FVoxelNodeFunctions::GetCurveValue(Params.Ocean, Variable_14);
				
				// *
				TVoxelRange<v_flt> Variable_23; // * output 0
				Variable_23 = Variable_5 * TVoxelRange<v_flt>(100.0f);
				
				// Smooth Step
				TVoxelRange<v_flt> Variable_20; // Smooth Step output 0
				Variable_20 = FVoxelMathNodeFunctions::SmoothStep(Variable_21, TVoxelRange<v_flt>(0.0f), Variable_14);
				
				// *
				TVoxelRange<v_flt> Variable_22; // * output 0
				Variable_22 = Variable_19 * Variable_20 * Variable_16;
				
				// Height Splitter
				TVoxelRange<v_flt> Variable_6; // Height Splitter output 0
				TVoxelRange<v_flt> Variable_7; // Height Splitter output 1
				TVoxelRange<v_flt> Variable_8; // Height Splitter output 2
				TVoxelRange<v_flt> Variable_9; // Height Splitter output 3
				{
					TVoxelStaticArray<TVoxelRange<v_flt>, 6> InputsArray;
					TVoxelStaticArray<TVoxelRange<v_flt>, 4> OutputsArray;
					InputsArray[0] = TVoxelRange<v_flt>(-10.0f);
					InputsArray[1] = TVoxelRange<v_flt>(5.0f);
					InputsArray[2] = TVoxelRange<v_flt>(10.0f);
					InputsArray[3] = TVoxelRange<v_flt>(10.0f);
					InputsArray[4] = TVoxelRange<v_flt>(30.0f);
					InputsArray[5] = TVoxelRange<v_flt>(5.0f);
					FVoxelMathNodeFunctions::HeightSplit(Variable_23, InputsArray, OutputsArray);
					Variable_6 = OutputsArray[0];
					Variable_7 = OutputsArray[1];
					Variable_8 = OutputsArray[2];
					Variable_9 = OutputsArray[3];
				}
				
				// +
				TVoxelRange<v_flt> Variable_15; // + output 0
				Variable_15 = Variable_14 + Variable_22;
				
				// Lerp
				TVoxelRange<v_flt> Variable_10; // Lerp output 0
				Variable_10 = FVoxelNodeFunctions::Lerp(TVoxelRange<v_flt>(0.0f), Variable_1, Variable_6);
				
				// Lerp
				TVoxelRange<v_flt> Variable_11; // Lerp output 0
				Variable_11 = FVoxelNodeFunctions::Lerp(Variable_10, Variable_2, Variable_7);
				
				// Float Curve: Mountains
				TVoxelRange<v_flt> Variable_4; // Float Curve: Mountains output 0
				Variable_4 = FVoxelNodeFunctions::GetCurveValue(Params.Mountains, Variable_15);
				
				// Lerp
				TVoxelRange<v_flt> Variable_12; // Lerp output 0
				Variable_12 = FVoxelNodeFunctions::Lerp(Variable_11, Variable_3, Variable_8);
				
				// Lerp
				TVoxelRange<v_flt> Variable_13; // Lerp output 0
				Variable_13 = FVoxelNodeFunctions::Lerp(Variable_12, Variable_4, Variable_9);
				
				// 2D Noise SDF.*
				TVoxelRange<v_flt> Variable_27; // 2D Noise SDF.* output 0
				Variable_27 = Variable_13 * Variable_18;
				
				// 2D Noise SDF.+
				BufferConstant.Variable_0 = Variable_27 + TVoxelRange<v_flt>(0.0f);
				
			}
		}
		void ComputeXYZWithoutCache(const FVoxelContextRange& Context, FOutputs& Outputs) const
		{
			Function0_XYZWithoutCache_Compute(Context, Outputs);
		}
		
		inline FBufferX GetBufferX() const { return {}; }
		inline FBufferXY GetBufferXY() const { return {}; }
		inline FOutputs GetOutputs() const { return {}; }
		
	private:
		FBufferConstant BufferConstant;
		
		const FParams& Params;
		
		FVoxelFastNoise _2D_Perlin_Noise_Fractal_1_Noise;
		TStaticArray<uint8, 32> _2D_Perlin_Noise_Fractal_1_LODToOctaves;
		FVoxelFastNoise _2D_Erosion_1_Noise;
		TStaticArray<uint8, 32> _2D_Erosion_1_LODToOctaves;
		FVoxelFastNoise _2D_IQ_Noise_1_Noise;
		TStaticArray<uint8, 32> _2D_IQ_Noise_1_LODToOctaves;
		FVoxelFastNoise _2D_Cubic_Noise_Fractal_1_Noise;
		TStaticArray<uint8, 32> _2D_Cubic_Noise_Fractal_1_LODToOctaves;
		
		///////////////////////////////////////////////////////////////////////
		//////////////////////////// Init functions ///////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_XYZWithoutCache_Init(const FVoxelGeneratorInit& InitStruct)
		{
		}
		
		///////////////////////////////////////////////////////////////////////
		////////////////////////// Compute functions //////////////////////////
		///////////////////////////////////////////////////////////////////////
		
		void Function0_XYZWithoutCache_Compute(const FVoxelContextRange& Context, FOutputs& Outputs) const
		{
			// Z
			TVoxelRange<v_flt> Variable_17; // Z output 0
			Variable_17 = Context.GetLocalZ();
			
			// 2D Noise SDF.-
			TVoxelRange<v_flt> Variable_26; // 2D Noise SDF.- output 0
			Variable_26 = Variable_17 - BufferConstant.Variable_0;
			
			// Set High Quality Value.*
			TVoxelRange<v_flt> Variable_25; // Set High Quality Value.* output 0
			Variable_25 = Variable_26 * TVoxelRange<v_flt>(0.2f);
			
			Outputs.Value = Variable_25;
		}
		
	};
	
	FNewVoxelGraphGeneratorInstance(UNewVoxelGraphGenerator& Object)
			: TVoxelGraphGeneratorInstanceHelper(
			{
				{ "Value", 1 },
			},
			{
			},
			{
			},
			{
				{
					{ "Value", NoTransformAccessor<v_flt>::Get<1, TOutputFunctionPtr<v_flt>>() },
				},
				{
				},
				{
				},
				{
					{ "Value", NoTransformRangeAccessor<v_flt>::Get<1, TRangeOutputFunctionPtr<v_flt>>() },
				}
			},
			{
				{
					{ "Value", WithTransformAccessor<v_flt>::Get<1, TOutputFunctionPtr_Transform<v_flt>>() },
				},
				{
				},
				{
				},
				{
					{ "Value", WithTransformRangeAccessor<v_flt>::Get<1, TRangeOutputFunctionPtr_Transform<v_flt>>() },
				}
			},
			Object)
		, Params(FParams
		{
			*Object.Default_Material.GetAssetName(),
			Object.Height,
			FVoxelRichCurve(Object.Islands.LoadSynchronous()),
			FVoxelRichCurve(Object.Land.LoadSynchronous()),
			Object.Mountain_erosion_strength,
			FVoxelRichCurve(Object.Mountains.LoadSynchronous()),
			FVoxelRichCurve(Object.Ocean.LoadSynchronous()),
			*Object.Snow.GetAssetName(),
			Object.Snow_Height,
			Object.Valleys_Height
		})
		, LocalValue(Params)
		, LocalMaterial(Params)
		, LocalUpVectorXUpVectorYUpVectorZ(Params)
		, LocalValueRangeAnalysis(Params)
	{
	}
	
	virtual void InitGraph(const FVoxelGeneratorInit& InitStruct) override final
	{
		LocalValue.Init(InitStruct);
		LocalMaterial.Init(InitStruct);
		LocalUpVectorXUpVectorYUpVectorZ.Init(InitStruct);
		LocalValueRangeAnalysis.Init(InitStruct);
	}
	
	template<uint32... Permutation>
	auto& GetTarget() const;
	
	template<uint32... Permutation>
	auto& GetRangeTarget() const;
	
private:
	FParams Params;
	FLocalComputeStruct_LocalValue LocalValue;
	FLocalComputeStruct_LocalMaterial LocalMaterial;
	FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ LocalUpVectorXUpVectorYUpVectorZ;
	FLocalComputeStruct_LocalValueRangeAnalysis LocalValueRangeAnalysis;
	
};

template<>
inline v_flt FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalValue::FOutputs::Get<v_flt, 1>() const
{
	return Value;
}
template<>
inline void FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalValue::FOutputs::Set<v_flt, 1>(v_flt InValue)
{
	Value = InValue;
}
template<>
inline FVoxelMaterial FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalMaterial::FOutputs::Get<FVoxelMaterial, 2>() const
{
	return MaterialBuilder.Build();
}
template<>
inline void FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalMaterial::FOutputs::Set<FVoxelMaterial, 2>(FVoxelMaterial Material)
{
}
template<>
inline v_flt FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ::FOutputs::Get<v_flt, 3>() const
{
	return UpVectorX;
}
template<>
inline void FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ::FOutputs::Set<v_flt, 3>(v_flt InValue)
{
	UpVectorX = InValue;
}
template<>
inline v_flt FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ::FOutputs::Get<v_flt, 4>() const
{
	return UpVectorY;
}
template<>
inline void FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ::FOutputs::Set<v_flt, 4>(v_flt InValue)
{
	UpVectorY = InValue;
}
template<>
inline v_flt FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ::FOutputs::Get<v_flt, 5>() const
{
	return UpVectorZ;
}
template<>
inline void FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalUpVectorXUpVectorYUpVectorZ::FOutputs::Set<v_flt, 5>(v_flt InValue)
{
	UpVectorZ = InValue;
}
template<>
inline TVoxelRange<v_flt> FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalValueRangeAnalysis::FOutputs::Get<v_flt, 1>() const
{
	return Value;
}
template<>
inline void FNewVoxelGraphGeneratorInstance::FLocalComputeStruct_LocalValueRangeAnalysis::FOutputs::Set<v_flt, 1>(TVoxelRange<v_flt> InValue)
{
	Value = InValue;
}
template<>
inline auto& FNewVoxelGraphGeneratorInstance::GetTarget<1>() const
{
	return LocalValue;
}
template<>
inline auto& FNewVoxelGraphGeneratorInstance::GetTarget<2>() const
{
	return LocalMaterial;
}
template<>
inline auto& FNewVoxelGraphGeneratorInstance::GetRangeTarget<0, 1>() const
{
	return LocalValueRangeAnalysis;
}
template<>
inline auto& FNewVoxelGraphGeneratorInstance::GetTarget<3, 4, 5>() const
{
	return LocalUpVectorXUpVectorYUpVectorZ;
}
#endif

////////////////////////////////////////////////////////////
////////////////////////// UCLASS //////////////////////////
////////////////////////////////////////////////////////////

UNewVoxelGraphGenerator::UNewVoxelGraphGenerator()
{
	bEnableRangeAnalysis = true;
}

TVoxelSharedRef<FVoxelTransformableGeneratorInstance> UNewVoxelGraphGenerator::GetTransformableInstance()
{
#if VOXEL_GRAPH_GENERATED_VERSION == 1
	return MakeVoxelShared<FNewVoxelGraphGeneratorInstance>(*this);
#else
#if VOXEL_GRAPH_GENERATED_VERSION > 1
	EMIT_CUSTOM_WARNING("Outdated generated voxel graph: NewVoxelGraphGenerator. You need to regenerate it.");
	FVoxelMessages::Warning("Outdated generated voxel graph: NewVoxelGraphGenerator. You need to regenerate it.");
#else
	EMIT_CUSTOM_WARNING("Generated voxel graph is more recent than the Voxel Plugin version: NewVoxelGraphGenerator. You need to update the plugin.");
	FVoxelMessages::Warning("Generated voxel graph is more recent than the Voxel Plugin version: NewVoxelGraphGenerator. You need to update the plugin.");
#endif
	return MakeVoxelShared<FVoxelTransformableEmptyGeneratorInstance>();
#endif
}

PRAGMA_GENERATED_VOXEL_GRAPH_END
