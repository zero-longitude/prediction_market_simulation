import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("build/results.csv")
cal = pd.read_csv("build/calibration.csv")
print(df.columns.tolist())

forecasters = ["Static(0.5)", "Bayes(1,1)", "Bayes(9,1)", "MLE"]

fig, axes = plt.subplots(3, 1, figsize=(12, 10), sharex=True)

# belief over time
for f in forecasters:
    axes[0].plot(df["t"], df[f"{f}_belief"], label=f)
axes[0].axhline(y=0.3, color="black", linestyle="--", label="true θ=0.3")
axes[0].set_ylabel("Belief")
axes[0].set_title("Belief over time")
axes[0].legend()

# brier score over time
for f in forecasters:
    axes[1].plot(df["t"], df[f"{f}_brier"], label=f, alpha=0.6)
axes[1].set_ylabel("Brier Score")
axes[1].set_title("Brier score per observation")
axes[1].legend()

# negative log score over time
for f in forecasters:
    axes[2].plot(df["t"], -df[f"{f}_log"], label=f, alpha=0.6)
axes[2].set_ylabel("Negative Log Score")
axes[2].set_title("Negative log score per observation")
axes[2].legend()

axes[2].set_xlabel("Observation")
plt.tight_layout()
plt.savefig("results.png", dpi=150)
plt.show()

# calibration plot
forecaster_names = cal["forecaster"].unique()
fig2, ax = plt.subplots(figsize=(6, 6))

ax.plot([0, 1], [0, 1], "k--", label="perfect calibration")
for name in forecaster_names:
    sub = cal[(cal["forecaster"] == name) & (cal["count"] > 0)]
    ax.plot(sub["mean_forecast"], sub["observed_freq"], "o-", label=name)

ax.set_xlabel("Mean forecast (p̄_b)")
ax.set_ylabel("Observed frequency (ō_b)")
ax.set_title("Calibration curve")
ax.set_xlim(0, 1)
ax.set_ylim(0, 1)
ax.legend()
plt.tight_layout()
plt.savefig("calibration.png", dpi=150)
plt.show()
