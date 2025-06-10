import asyncio
import yfinance as yf
from agents import Agent, Runner, function_tool

@function_tool
def get_stock_price(stock_symbol: str) -> float:
  """
  Get the current stock price for a given stock symbol.
  """
  stock = yf.Ticker(stock_symbol)
  return stock.history(period="1d")['Close'].iloc[-1]

template = """
You are a financial assistant with access to real-time stock prices.
If the question is about stock prices, use the `get_stock_price` tool provided to get the latest value.

Question: {question}
"""

agent = Agent(
  name="Financial Assistant",
  instructions=template,
  tools=[get_stock_price]
)

async def main():
  result = await Runner.run(agent, input="What is the current price of AAPL stock?")
  print(result.final_output)

if __name__ == "__main__":
  asyncio.run(main())
