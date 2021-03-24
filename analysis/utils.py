# common utility functions for calculations
import warnings
warnings.filterwarnings("ignore")


def getProfitFactor(df):
    onlyProfit = df[df["PL"] > 0]
    totalProfit = onlyProfit["PL"].sum()
    onlyLoss = df[df["PL"] < 0]
    totalLoss = onlyLoss["PL"].sum()
    return abs(totalProfit/totalLoss)


def getProfitFactorForAllPermutations(df):
    for i in range(int(df["Momentum"].min()), int(df["Momentum"].max()), 6):
        for j in range(int(df["Range Expansion Index"].min()), int(df["Range Expansion Index"].max()), 11):
            for k in range(int(df["Normalized Volume"].min()), int(df["Normalized Volume"].max()), 51):
                res = getProfitFactor(df[(df["Momentum"] > i) & (df["Range Expansion Index"] > j) & (df["Normalized Volume"] > k)])
                if res > 1.5:
                    print(i, j, k, res)

                
def getReasonableMinMaxValuesOfIndicatorBasedOnTradeNumbers(df, indicator):
    for i in range(int(df[indicator].min()), int(df[indicator].max())):
        if len(df[df[indicator] > i]) < 500: 
            print(f"{indicator} je vetsi nez " + str(i) + ", obchodu je " + str(len(df[df[indicator] > i])))
    for i in range(int(df[indicator].max()), int(df[indicator].min()), -1):
        if len(df[df[indicator] < i]) < 500: 
            print("{indicator} je mensi nez " + str(i) + ", obchodu je " + str(len(df[df[indicator] < i])))
